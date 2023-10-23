// See https://aka.ms/new-console-template for more information
//Console.WriteLine("Hello, World!");



var Execute = (string[] filenames) =>
{
	foreach( var filename in filenames )
	{
		if (
		!filename.EndsWith(".h") ||
		!File.Exists(filename)
		)
			continue;

		var CHeader = filename.Substring( Math.Max( filename.LastIndexOf('/'), filename.LastIndexOf('\\') ) + 1 );

		var CSourcePath = filename.Replace(".h", ".c");
		if ( File.Exists(CSourcePath) )
			continue;

		var lines = File.ReadAllLines(filename);

		var APISet = "";
		{
			var prefix = "extern ENGINE_API ";
			var suffix = ";";

			foreach( var line in lines )
				if ( line.StartsWith(prefix) )
				{
					APISet = line.Replace(prefix, "").Replace(suffix, "");
					break;
				}
		}

		var IsUpperCaseLetter = (char code) =>
		{
			return code >= 'A' && code <= 'Z';
		};

		var IsLowerCaseLetter = (char code) =>
		{
			return code >= 'a' && code <= 'z';
		};

		var IsFunctionPointer = (string text) =>
		{
			if ( text.Length < 4 )
				return false;

			if ( text[0] != '\t' )
				return false;

			if (
				!IsLowerCaseLetter( text[1] ) &&
				!IsUpperCaseLetter( text[1] )
			)
				return false;

			if ( !text.Contains("(*") )
				return false;

			return true;
		};

		var FunctionPointers	= new List<string>();
		var Definitions			= new List<string>();

		foreach( var line in lines )
		{
			if ( !IsFunctionPointer(line) )
				continue;

			var Return = "";
			{
				var begin	= 1;
				var end		= line.IndexOf('(');

				Return = line.Substring( begin, end - begin );
			}

			var Name = "";
			{
				var begin	= line.IndexOf('(') + 2;
				var end		= line.IndexOf(')');

				Name = line.Substring( begin, end - begin );
			}

			var Arguments = "";
			{
				var begin	= line.LastIndexOf('(') + 1;
				var end		= line.LastIndexOf(')');

				Arguments = line.Substring( begin, end - begin );
			}

			var Definition = $"static {Return} {Name}({Arguments})" + "\n{\n\n}";

			FunctionPointers.Add(Name);
			Definitions.Add(Definition);
		}

		var sb = new System.Text.StringBuilder();

		sb.AppendLine($"#include \"{CHeader}\"");
		sb.AppendLine();
		sb.AppendLine();
		sb.AppendLine();

		foreach( var Definition in Definitions )
		{
			sb.AppendLine(Definition);
			sb.AppendLine();
		}

		sb.AppendLine();
		sb.AppendLine($"{APISet} =");
		sb.AppendLine("{");
		foreach ( var FunctionPointer in FunctionPointers ) sb.AppendLine($"\t{FunctionPointer},");
		sb.AppendLine("};");

		File.WriteAllText( CSourcePath, sb.ToString() );
	}
};


Execute(
	new[]
	{
        @"C:\Users\oneha\source\repos\LevelUp\C\Engine\Source\Runtime\Core\Container\LinkedList.h",
	}
);

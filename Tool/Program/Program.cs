var GetFileName = (string path) =>
{
    return path.Substring(path.LastIndexOf('\\') + 1);
};


var path = @"C:\Dev\LevelUp\CPlusPlus\Engine\Source\Runtime\Platform\SAL";
var prefix = "#include \"../Runtime/Platform/SAL/";


foreach(var file in Directory.GetFiles(path, "*.h"))
    Console.WriteLine(prefix + GetFileName(file) + "\"");
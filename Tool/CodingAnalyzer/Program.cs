

var Source = @"C:\Dev\LevelUp\CPlusPlus\Engine";

var filters = new[] { "*.h", "*.c", "*.cpp" };

var totalCodingLines = 0;
foreach(var filter in filters)
    foreach(var file in Directory.EnumerateFiles(Source, filter, SearchOption.AllDirectories))
    {
        foreach(var line in File.ReadAllLines(file))
        {
            if (line.Length > 0)
                totalCodingLines++;
        }
    }

Console.WriteLine($"Total coding lines: {totalCodingLines}");
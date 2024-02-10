#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

void checkFile(const string &filePath)
{
    ifstream file(filePath, ios::binary);
    if (!file)
    {
        return;
    }
    char c;
    while (file.get(c))
    {
        if (c == '\0')
        {
            return;
        }
    }
    file.clear();
    file.seekg(0, ios::beg);
    char bom[3] = {0};
    file.read(bom, 3);
    if (bom[0] == (char)0xEF && bom[1] == (char)0xBB && bom[2] == (char)0xBF)
    {
        file.seekg(3, ios_base::beg);
    }
    else
    {
        file.seekg(0, ios_base::beg);
    }
    char c2;
    while (file.get(c2))
    {
        if (static_cast<unsigned char>(c2) > 127)
        {
            cout << filePath << endl;
            return;
        }
    }
}

void checkDir(const string &dirPath)
{
    for (const auto &path : filesystem::recursive_directory_iterator(dirPath))
    {
        if (path.is_regular_file())
        {
            checkFile(path.path().string());
        }
    }
}

signed main(int argc, char *argv[])
{
    if (argc == 1)
    {
        cout << "Usage: " << argv[0] << " <path1> <path2> <...>" << endl;
    }
    for (int i = 1; i < argc; i++)
    {
        if (filesystem::is_directory(argv[i]))
        {
            checkDir(argv[i]);
        }
        else
        {
            checkFile(argv[i]);
        }
    }
    return 0;
}
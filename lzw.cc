// Lempel-Ziv-Welch encoding/decoding
// not entirely idiomatic C++.
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <sstream>
#include <vector>

template <typename T>
void printvec(std::vector<T> v)
{
    for (int i = 0; i < v.size(); ++i)
        std::cout << v[i] << " \n"[i+1 == v.size()];
}

std::vector<int> split(std::string &s)
{
    std::vector<int> res;
    std::stringstream ss(s);

    int x;
    while (ss >> x)
        res.push_back(x);

    return res;
}

std::vector<int> lzw_encode(std::string &s);
std::string lzw_decode(std::vector<int> &comp);

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cout << "usage: " << argv[0] << " -e|-d\n";
        return 1;
    }

    if (argv[1][0] != '-' || (argv[1][0] == '-' && argv[1][1] != 'e' && argv[1][1] != 'd')) {
        std::cout << "error: expected -e or -d\n";
        return 1;
    }

    char opt = argv[1][1];

    // std::cin is not enough, so do this trick
    std::string text(std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>());

    if (opt == 'e') {
        std::vector<int> comp = lzw_encode(text);
        printvec(comp);
    }

    // Input is expected to be string of codes
    // Example: 84 79 66 69 79 82 78 79 84 256 258 260 265 259 261 263
    else if (opt == 'd') {
        std::vector<int> comp = split(text);
        std::string dec = lzw_decode(comp);
        std::cout << dec << "\n";
    }

    return 0;
}

std::vector<int> lzw_encode(std::string &s)
{
    int free_idx = 256;
    std::vector<int> res;
    std::map<std::string, int> dict;
    for (int i = 0; i < 256; ++i)
        dict[std::string(1, i)] = i;

    std::string w = "";
    for (char c : s) {
        std::string new_w = w + c;
        if (dict.count(new_w))
            w = new_w;
        else {
            res.push_back(dict[w]);
            dict[new_w] = free_idx++;
            w = std::string(1, c);
        }
    }

    if (!w.empty())
        res.push_back(dict[w]);

    return res;
}

std::string lzw_decode(std::vector<int> &comp)
{
    std::string res = "";
    int free_idx = 256;
    std::map<int, std::string> dict;
    for (int i = 0; i < 256; ++i)
        dict[i] = std::string(1, i);

    std::string w(1, comp[0]);
    std::string next_string;

    res += w;
    for (int i = 1; i < comp.size(); ++i) {
        int encoded = comp[i];
        if (dict.count(encoded))
            next_string = dict[encoded];
        else if (encoded == free_idx)
            next_string = w + w[0];

        res += next_string;
        dict[free_idx++] = w + next_string[0];
        w = next_string;
    }

    return res;
}

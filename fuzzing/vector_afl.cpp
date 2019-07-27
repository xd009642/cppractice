#include <vector.hpp>
#include <iostream>
#include <string>

int main() {
    std::string in;
    std::cin >> in;
    xd::vector<char> fuzz_vec;
    for(size_t i=0; i<in.size(); i++) {
        if(in[i] == 'a') {
            //assign    
            char data = in[i];
            size_t size = in[i];
            if(i + 1 < in.size()) {
                size = in[i+1];
            }
            fuzz_vec.assign(size, data);
        } else if(in[i] == 'e') {
            // emplace
            fuzz_vec.emplace_back(in[i]);
        } else if(in[i] == 'r') {
            //resize
            size_t size = in[i];
            fuzz_vec.resize(size);
        } else if(in[i] == 'R') {
            // reserve
            size_t size = in[i];
            fuzz_vec.reserve(size);
        } else if(in[i] == 'p') {
            // push
            fuzz_vec.push_back(in[i]);
        } else if(in[i] == 'P') {
            // pop
            fuzz_vec.pop_back();
        } else if(in[i] == 'c') {
            // clear
            fuzz_vec.clear();
        } else if(in[i] == 's') {
            // shrink to fit
            fuzz_vec.shrink_to_fit();
        } else if(in[i] == 'E') {
            // empty
            fuzz_vec.empty();
        } else if(in[i] == 'S') {
            fuzz_vec.size();
        }

    }
}


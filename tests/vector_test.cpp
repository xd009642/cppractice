#include "vector.hpp"
#include <iostream>
#include <string>

class count_dtors {
public:
    static int32_t count;
    count_dtors() {
        foo = "bar";
        count++;
    }
    count_dtors(const count_dtors& other) {
        count++;
    }
    ~count_dtors() {
        count--;
    }
private:
    // Just hear to make sure I don't leak
    std::string foo;
};

int32_t count_dtors::count = 0;

void assert(bool condition, const std::string& message) {
    if(!condition) {
        std::cout<<message<<std::endl;
        exit(-1);
    }
}


void check_dtors() {
    xd::vector<count_dtors> list((size_t)2);
    list.clear();
    assert(count_dtors::count == 0, "Leaked object! "+ std::to_string(count_dtors::count));
    list.reserve(10);
    list.clear();
    assert(count_dtors::count == 0, "Too many deletes! "+ std::to_string(count_dtors::count));
}

void test_emplace_back() {
    xd::vector<std::string> list;
    list.emplace_back(3, 'C');
    assert(list[0] == "CCC", "Emplace back doesn't work");
}

void test_insert() {
    xd::vector<uint32_t> list = {1, 2};
    list.insert(list.begin(), 0);
    uint32_t curr = 0;
    for(const auto& i: list) {
        assert(i==curr, "Insert failed "+std::to_string(i));
        curr++;
    }
}

int main() {
    xd::vector<uint32_t> int_list((size_t)10, 45);
    uint32_t test = int_list.at(2);
    for(int i=0; i<5; i++) {
        assert(int_list.at(i) == 45, "Incorrect data at index "+std::to_string(i));
    }
    assert(int_list.size() == 10, "Incorrect vector size");
    
    int_list.at(2) = 54;
    assert(int_list.at(2) == 54, "Correctly mutate data");

    int_list.push_back(32);
    assert(int_list.at(int_list.size()-1)==32, "Append data correctly");
    assert(int_list.size()==11, "Failed to update size");
    try {
        int_list.at(11);
        assert(false, "Accessed invalid index");
    } catch(const std::out_of_range& e) {
    }

    assert(!int_list.empty(), "vector<T>::empty works pt1");
    int_list.clear();
    assert(int_list.empty(), "vector<T>::empty works pt2");
    assert(int_list.size() == 0, "List now empty");

    check_dtors();
    xd::vector<uint32_t> int_list2 = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
    for(int i=0; i<int_list2.size(); i++) {
        assert(int_list2[i]==i, "Int list value is wrong! Index: " + 
                std::to_string(i)+" value: " + std::to_string(int_list2[i]));
    }
    size_t count =0;
    for(const auto i:int_list2) {
        count++;
        assert(int_list2[i]==i, "Iterators don't work: " + 
                std::to_string(i)+" value: " + std::to_string(int_list2[i]));
    }
    assert(count == int_list2.size(), "Didn't iterate over all elements");
    int_list2 = {2,2,2,2,2};
    for(int i=0; i<int_list2.size(); i++) {
        assert(int_list2[i]==2, "Failed to assign from initializer_list");
    }

    test_emplace_back();

    test_insert();

    std::cout<<"Test passed"<<std::endl;
    return 0;
}

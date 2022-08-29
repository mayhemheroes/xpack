#include <iostream>
//#include "xpack/json.h" // 包含这个头文件
#include "json.h"
#include <string>
using namespace std;

struct User {
    int64_t id;
    string  name;
    string  mail;
    User(int64_t i=0, const string& n="", const string& m=""):id(i),name(n),mail(m){}
    XPACK(O(id, name, mail)); // 添加宏定义XPACK在结构体定义结尾
};

struct Group {
    string  name;
    int64_t master;
    map<int, string> flags;
    vector<User> members;
    User         arrays[2];
    XPACK(O(name, master, flags, members, arrays)); // 添加宏定义XPACK在结构体定义结尾
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{ 
    std::string buf(reinterpret_cast<const char*>(data), size);
    buf.push_back('\0');
    Group g;
    g.name = "C++";
    g.master = 2019;
    g.members.resize(2);
    g.members[0] = User(1, "Jack", buf.c_str());
    g.members[1] = User(2, buf.c_str(), "pony@xpack.com");
    g.arrays[0] = g.members[0];
    g.arrays[1] = g.members[1];
    g.flags[1] = "a";
    g.flags[2] = "b";

    string json = xpack::json::encode(g);      // 结构体转json
    cout<<json<<endl;

    Group n;
    xpack::JsonDecoder jd(json, false);
    
    jd["name"].decode(NULL, n.name, NULL);
    cout<<"name is:"<<n.name<<endl;

    xpack::JsonDecoder&flags = jd["flags"];
    for (xpack::JsonDecoder::Iterator it=flags.Begin(); it!=flags.End(); ++it) {
        string f;
        it.Val().decode(NULL, f, NULL);
        cout<<"    "<<it.Key()<<':'<<f<<endl;
    }

    return 0;
}

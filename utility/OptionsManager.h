#ifndef _OPTIONS_MANAGER_H
#define _OPTIONS_MANAGER_H

#include <memory>

using namespace std;

class Value;
class Hash;

class OptionsManager {
   
public:
   static OptionsManager& instance();
   ~OptionsManager();

   Hash *getOptions() const;
   
private:
   OptionsManager();
   static auto_ptr<OptionsManager> s_instance;
   Value *value;
};

#endif

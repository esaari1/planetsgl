#include "OptionsManager.h"
#include <fstream>
#include "parser/Parser.h"

auto_ptr<OptionsManager> OptionsManager::s_instance;

OptionsManager& OptionsManager::instance() {
   if(s_instance.get() == 0) {
      s_instance.reset(new OptionsManager);
   }
   return *s_instance;
}

OptionsManager::OptionsManager() {
   std::ifstream fin("config.ini", std::ios::in);
   Tokenizer tok(&fin);
   Parser parser(&tok);
   value = parser.readValue();
}

OptionsManager::~OptionsManager() {
   delete value;
}

Hash *OptionsManager::getOptions() const {
   return value->getHash();
}

//
// Created by connor on 11/18/18.
//

#include <cstring>
#include "StringScanner.h"

namespace DsprFrontend {

    StringScanner::StringScanner(Ref<Sova::String> str) {
        this->theString = str;
    }

    bool StringScanner::EqualsUntil(const char *str, char separator) {
        auto strLen = std::strlen(str);
        for (int i=0;i<strLen;i++){
            if (i>this->theString->Length()) continue;
            if (this->theString->CharAt(i+this->index) != str[i]) return false;
        }

        if (this->theString->CharAt(strLen+this->index) != separator) return false;

        return true;
    }

    void StringScanner::Advance(unsigned int i){
        this->index += i;
    }

    void StringScanner::CleanCstr() {
        delete this->theCstr;
    }

    char* StringScanner::CstrAndAdvanceUntil(char separator) {
        int length = -1;

        for(int i=0; i+this->index < this->theString->Length(); i++)
        {
            if (this->theString->CharAt(i+this->index) == separator) {
                length = i; break;
            }
        }
        if (length == -1) { this->index = this->theString->Length(); return nullptr; }
        this->theCstr = new char[length+1];
        this->theCstr[length] = '\0';
        for(int i=0; i < length; i++)
        {
            this->theCstr[i] = this->theString->CharAt(i+this->index);
        }
        this->Advance(length+1);
        return this->theCstr;
    }

    bool StringScanner::IsAtEnd(){
        return (this->index >= this->theString->Length());
    }
}
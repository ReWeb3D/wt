/*
 * Copyright (C) 2011 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <Wt/WJavaScriptPreamble>

// TG: my stuff

namespace Wt {

WJavaScriptPreamble::WJavaScriptPreamble(JavaScriptScope scope,
					 JavaScriptObjectType type,
					 const char *name, const char *src)
  : scope(scope),
    type(type),
    name(name),
    src(src)
{ 

    //// TG: debug test: add linebreaks 
    //std::string needle (";{}");
    //std::string str (src);

    //int position = str.find_first_of(needle);
    //while (position != std::string::npos)
    //{
    //    if (position >= str.size()-1) break;
    //    str.insert(position+1,"\n");
    //    position = str.find_first_of(needle,position+1);
    //}

    //// memory leak!
    //char * writable = new char[str.size() + 1];
    //std::copy(str.begin(), str.end(), writable);
    //writable[str.size()] = '\0'; // don't forget the terminating 0

    //this->src = writable ;
}

}

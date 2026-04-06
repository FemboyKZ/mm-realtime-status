#ifndef _INCLUDE_JSON_BUILDER_H_
#define _INCLUDE_JSON_BUILDER_H_

#include <string>

std::string JsonEscape(const char *str);
std::string BuildPayloadJson();
std::string BuildHibernateJson();

#endif // _INCLUDE_JSON_BUILDER_H_

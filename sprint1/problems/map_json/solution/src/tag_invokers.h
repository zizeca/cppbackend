#pragma once

#ifndef __TAG_INVOKERS_H__
#define __TAG_INVOKERS_H__

#include <boost/json.hpp>
#include "model.h"



namespace model {

void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Building const& building);
void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Office const& office);
void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Road const& road);
void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Map const& map);

}
#endif // __TAG_INVOKERS_H__
#ifndef __TAG_INVOKERS_H__
#define __TAG_INVOKERS_H__


#include <boost/json.hpp>
#include "model.h"

namespace model {

void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Building const& building);
void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Office const& office);
void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Road const& road);
void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Map const& map);



//model::Building tag_invoke(boost::json::value_to_tag<model::Building>, boost::json::value& jv);
//model::Office tag_invoke(boost::json::value_to_tag<model::Office>, boost::json::value& jv);
//model::Road tag_invoke(boost::json::value_to_tag<model::Road>, boost::json::value& jv);
//model::Map tag_invoke(boost::json::value_to_tag<model::Map>, boost::json::value& jv);

};  // namespace json_loader

#endif // __TAG_INVOKERS_H__
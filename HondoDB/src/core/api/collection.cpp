#include "collection.h"


namespace hondo {

/*CollectionFunctionResult Collection::create()
{}

CollectionFunctionResult Collection::retrieve()
{}

CollectionFunctionResult Collection::update()
{}

CollectionFunctionResult Collection::destroy()
{}*/

Collection::Collection(Connection* s_connection, util::String s_name)
{}

Collection::~Collection()
{}

CollectionFunctionResult Collection::retrieve_all()
{
	return CollectionFunctionResult();
}

/*CollectionFunctionResult Collection::nuke()
{}*/

}
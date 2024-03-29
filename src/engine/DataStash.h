#pragma once

#include <stdexcept>
#include "IWritable.h"
#include "LinkedList.h"


template<typename T>
concept IWritableTemplate = std::is_base_of<IWritable, T>::value;


template<IWritableTemplate T>
class DataStash : LinkedList<T> {
public:

private:

};


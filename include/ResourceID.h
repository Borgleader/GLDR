#pragma once

template<typename IDType, typename Destructor>
class ResourceID
{
public:
	ResourceID(IDType id, Destructor dtor): id_(id), dtor_(dtor)
	{
	}

	ResourceID(ResourceID&& other): id_(other.id_), dtor_(other.dtor_)
	{
	}

	IDType get()
	{
		return id_;
	}

	virtual ~ResourceID()
	{
		dtor_();
	}

protected:
	IDType id_;
	Destructor dtor_;
};

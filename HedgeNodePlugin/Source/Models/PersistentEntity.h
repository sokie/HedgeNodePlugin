class PersistentEntity
{
public:
	virtual ~PersistentEntity() {}

	//persistence stuff
	virtual bool Save() = 0;
	virtual bool Update() = 0;
	virtual bool Remove() = 0;
	
protected:

};
#ifndef _ENTITY_PROPERTY_HPP_
#define _ENTITY_PROPERTY_HPP_

#include <string>
#include <OgrePrerequisites.h>

class CEntity;

class CEntityProperty {
public:
  enum EPropertyTypes {
    PT_INT,
    PT_REAL,
    PT_STRING,
  };
private:
  CEntity &mParent;
  const EPropertyTypes mType;
  const int mID;

protected:
  CEntityProperty(CEntity &parent, const EPropertyTypes type, const int id)
    : mParent(parent),
      mType(type),
      mID(id) {
  }
  virtual ~CEntityProperty() {}

public:
  const EPropertyTypes getType() const {return mType;}
  const int getID() const {return mID;}

protected:
  void dataChanged();
};

template<class Data, CEntityProperty::EPropertyTypes Type>
class CEntityPropertyTemplate : public CEntityProperty {
private:
  Data mData;
public:
  CEntityPropertyTemplate(CEntity &parent, const int id, const Data &initialValue)
    : CEntityProperty(parent, Type, id),
      mData(initialValue) {
  }

  const Data &getData() const {return mData;}
  virtual void setData(const Data &data) {
    if (mData != data) {
      mData = data;
      dataChanged();
    }
  }
  void addData(const Data &data) {
    setData(mData + data);
  }
};
template<class Data, CEntityProperty::EPropertyTypes Type>
class CEntityPropertyBoundedTemplate : public CEntityPropertyTemplate<Data, Type> {
private:
  Data mMin;
  Data mMax;
public:
  CEntityPropertyBoundedTemplate(CEntity &parent,
				 const int id,
				 const Data min,
				 const Data max,
				 const Data initialValue)
    : CEntityPropertyTemplate<Data, Type>(parent, id, initialValue),
      mMin(min),
      mMax(max) {
  }

  virtual void setData(const Data &data) {
   Data toSet = std::min<const Data>(std::max<const Data>(mMin, data), mMax);
   CEntityPropertyTemplate<Data, Type>::setData(toSet);
  }

  void setMinimum(const Data min) {mMin = min;}
  void setMaximum(const Data max) {mMax = max;}

private:
  void refresh() {
    CEntityPropertyTemplate<Data, Type>::setData(CEntityPropertyTemplate<Data, Type>::getData());
  }
};

typedef CEntityPropertyBoundedTemplate<int, CEntityProperty::PT_INT> CEntityBoundedIntProperty;
typedef CEntityPropertyTemplate<int, CEntityProperty::PT_INT> CEntityIntProperty;
typedef CEntityPropertyTemplate<Ogre::Real, CEntityProperty::PT_REAL> CEntityRealType;
typedef CEntityPropertyTemplate<std::string, CEntityProperty::PT_STRING> CEntityStringType;

#endif
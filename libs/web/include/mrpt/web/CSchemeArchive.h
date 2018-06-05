#pragma once
#include <mrpt/serialization/CSerializable.h>
#include <mrpt/serialization/CSchemeArchiveBase.h>
#include <string_view>
#include <string>
#include <memory>

namespace mrpt::web
{
template <typename SCHEME_CAPABLE>
class CSchemeArchive : public mrpt::serialization::CSchemeArchiveBase
{
  public:
    CSchemeArchive(SCHEME_CAPABLE& val):m_val(val) {}
    //Virtual assignment operators
    virtual mrpt::serialization::CSchemeArchiveBase &operator=(int val) override
    {
        m_val = val;
        return *this;
    }
    virtual mrpt::serialization::CSchemeArchiveBase &operator=(float val) override
    {
        m_val = val;
        return *this;
    }
    virtual mrpt::serialization::CSchemeArchiveBase &operator=(double val) override
    {
        m_val = val;
        return *this;
    }
    virtual mrpt::serialization::CSchemeArchiveBase &operator=(std::nullptr_t val) override
    {
        m_val = val;
        return *this;
    }
    virtual mrpt::serialization::CSchemeArchiveBase &operator=(std::string val) override
    {
        m_val = val;
        return *this;
    }
    virtual mrpt::serialization::CSchemeArchiveBase &operator=(bool val) override
    {
        m_val = val;
        return *this;
    }

    virtual explicit operator int() const override
    {
        return m_val.asInt();
    }
    virtual explicit operator float() const override
    {
        return m_val.asFloat();
    }
    virtual explicit operator double() const override
    {
        return m_val.asDouble();
    }
    virtual explicit operator bool() const override
    {
        return m_val.asBool();
    }
    virtual explicit operator std::string() const override
    {
        return m_val.asString();
    }

    virtual mrpt::serialization::CSchemeArchiveBase &operator=(mrpt::serialization::CSerializable& obj) override
    {
        mrpt::serialization::CSchemeArchiveBase::ReadObject(*this, obj);
        return *this;
    }
    virtual void asSerializableObject(mrpt::serialization::CSerializable& obj) override
    {
        mrpt::serialization::CSchemeArchiveBase::WriteObject(*this, obj);
        return;
    }

    virtual mrpt::serialization::CSchemeArchiveBase::Ptr operator[](size_t idx) override
    {
        return std::make_shared<CSchemeArchive<SCHEME_CAPABLE>>(m_val[(int)idx]);
    }
    virtual mrpt::serialization::CSchemeArchiveBase::Ptr operator[](std::string str) override
    {  
        return std::make_shared<CSchemeArchive<SCHEME_CAPABLE>>(m_val[std::string(str)]);
    }

    // no longer required
    SCHEME_CAPABLE get()
    {
        return m_val;
    }
    private:
        SCHEME_CAPABLE& m_val;
};

}
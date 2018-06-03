#pragma once
#include <mrpt/serialization/CSerializable.h>
#include <mrpt/serialization/CSchemeArchiveBase.h>
#include <string_view>
#include <string>
#include <memory>
#include <iostream>

namespace mrpt::web
{
template <typename SCHEME_CAPABLE>
class CSchemeArchive : public mrpt::serialization::CSchemeArchiveBase
{
    private:
    CSchemeArchive(SCHEME_CAPABLE& m_val_):m_val(&m_val_) {
        m_base = false;
    }
    public:
    CSchemeArchive()
    {
        m_val = new SCHEME_CAPABLE;
        m_base = true;
    }
    ~CSchemeArchive()
    {
        for(auto e : m_map_index) delete e.second;
        for(auto e : m_map_key) delete e.second;
        if(m_base)
                delete m_val;
    }
    //Virtual assignment operators
    virtual mrpt::serialization::CSchemeArchiveBase &operator=(int val) override
    {
        *m_val = val;
        return *this;
    }

    virtual mrpt::serialization::CSchemeArchiveBase &operator=(float val) override
    {
        *m_val = val;
        return *this;
    }

    virtual mrpt::serialization::CSchemeArchiveBase &operator=(double val) override
    {
        *m_val = val;
        return *this;
    }
    virtual mrpt::serialization::CSchemeArchiveBase &operator=(std::nullptr_t val) override
    {
        *m_val = val;
        return *this;
    }
    virtual mrpt::serialization::CSchemeArchiveBase &operator=(std::string_view val) override
    {
        *m_val = std::string(val);
        return *this;
    }
    virtual mrpt::serialization::CSchemeArchiveBase &operator[](size_t idx) override
    {
        int idx_int = idx;
        if(m_map_index.find(idx_int) == m_map_index.end()) m_map_index[idx_int] = new CSchemeArchive<SCHEME_CAPABLE>((*m_val)[idx_int]);
        return *m_map_index[idx_int];
    }
    virtual mrpt::serialization::CSchemeArchiveBase &operator[](std::string_view key) override
    {  
        std::string key_str(key);
        if(m_map_key.find(key_str) == m_map_key.end()) m_map_key[key_str] = new CSchemeArchive<SCHEME_CAPABLE>((*m_val)[key_str]);
        return *m_map_key[key_str];
    }
    mrpt::serialization::CSchemeArchiveBase &operator=(mrpt::serialization::CSerializable& obj) override
    {
        mrpt::serialization::CSchemeArchiveBase::ReadObject(*this, obj);
    }
    SCHEME_CAPABLE get()
    {
        return *m_val;
    }
    private:
        SCHEME_CAPABLE* m_val;
        std::map<std::string, CSchemeArchive<SCHEME_CAPABLE>*> m_map_key;
        std::map<int, CSchemeArchive<SCHEME_CAPABLE>*> m_map_index;
        bool m_base;
};

}
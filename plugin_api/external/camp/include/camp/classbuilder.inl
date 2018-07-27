/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/


namespace camp
{
//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T>::ClassBuilder(Class& target) :
    ClassBuilderBase(target)
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename U>
ClassBuilder<T>& ClassBuilder<T>::base()
{
    // Compute the offset to apply for pointer conversions
    const T* asDerived = reinterpret_cast<const T*>(1);
    const U* asBase = static_cast<const U*>(asDerived);
    const int offset = static_cast<int>(reinterpret_cast<const char*>(asBase) - reinterpret_cast<const char*>(asDerived));

    // Add the base class
    addBase(const_cast<Class&>(classByType<U>()), offset);

    // Done
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::property(const char* name, F accessor)
{
    // Find the factory which will be able to construct a camp::Property from an accessor of type F
    typedef detail::PropertyFactory1<T, F> Factory;

    // Construct and add the metaproperty
    addProperty(Factory::get(name, accessor));

    // Done
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F1, typename F2>
ClassBuilder<T>& ClassBuilder<T>::property(const char* name, F1 accessor1, F2 accessor2)
{
    // Find the factory which will be able to construct a camp::Property from accessors of type F1 and F2
    typedef detail::PropertyFactory2<T, F1, F2> Factory;

    // Construct and add the metaproperty
    addProperty(Factory::get(name, accessor1, accessor2));

    // Done
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F1, typename F2, typename F3>
ClassBuilder<T>& ClassBuilder<T>::property(const char* name, F1 accessor1, F2 accessor2, F3 accessor3)
{
    // Find the factory which will be able to construct a camp::Property from accessors of type F1, F2 and F3
    typedef detail::PropertyFactory3<T, F1, F2, F3> Factory;

    // Construct and add the metaproperty
    addProperty(Factory::get(name, accessor1, accessor2, accessor3));

    // Done
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::function(const char* name, F function)
{
    // Get a uniform function type from F, whatever it really is
    typedef typename boost::function_types::function_type<F>::type Signature;

    // Construct and add the metafunction
    addFunction(new detail::FunctionImpl<Signature>(name, function));

    // Done
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::function(const char* name, boost::function<F> function)
{
    addFunction(new detail::FunctionImpl<F>(name, function));

    // Done
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F1, typename F2>
ClassBuilder<T>& ClassBuilder<T>::function(const char* name, F1 function1, F2 function2)
{
    // Get uniform function types from F1 and F2, whatever they really are
    typedef typename boost::function_types::function_type<F1>::type Signature1;
    typedef typename boost::function_types::function_type<F2>::type Signature2;

    // Construct and add the metafunction
    addFunction(new detail::FunctionImpl<Signature1, Signature2>(name, function1, function2));

    // Done
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T>& ClassBuilder<T>::tag(const char* name)
{
    // Add the tag
    addTag(name, Value::nothing);

    // Done
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename U>
ClassBuilder<T>& ClassBuilder<T>::tag(const char* name, const U& value)
{
    // For the special case of Getter<Value>, the ambiguity between both constructors
    // cannot be automatically solved, so let's do it manually
    typedef typename boost::mpl::if_c<detail::FunctionTraits<U>::isFunction, boost::function<Value (T&)>, Value>::type Type;

    // Add the tag
    addTag(name, detail::Getter<Value>(Type(value)));

    // Done
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T>& ClassBuilder<T>::readable(bool value)
{
    // Make sure we have a valid property
    assert(m_currentProperty != nullptr);

    m_currentProperty->m_readable = detail::Getter<bool>(value);

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::readable(F function)
{
    // Make sure we have a valid property
    assert(m_currentProperty != nullptr);

    m_currentProperty->m_readable = detail::Getter<bool>(boost::function<bool (T&)>(function));

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T>& ClassBuilder<T>::writable(bool value)
{
    // Make sure we have a valid property
    assert(m_currentProperty != nullptr);

    m_currentProperty->m_writable = detail::Getter<bool>(value);

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::writable(F function)
{
    // Make sure we have a valid property
    assert(m_currentProperty != nullptr);

    m_currentProperty->m_writable = detail::Getter<bool>(boost::function<bool (T&)>(function));

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T>& ClassBuilder<T>::callable(bool value)
{
    // Make sure we have a valid function
    assert(m_currentFunction != nullptr);

    m_currentFunction->m_callable = detail::Getter<bool>(value);

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::callable(F function)
{
    // Make sure we have a valid function
    assert(m_currentFunction != nullptr);

    m_currentFunction->m_callable = detail::Getter<bool>(boost::function<bool (T&)>(function));

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T>& ClassBuilder<T>::constructor0()
{
    addConstructor(new detail::ConstructorImpl0<T>);
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename A0>
ClassBuilder<T>& ClassBuilder<T>::constructor1()
{
    addConstructor(new detail::ConstructorImpl1<T, A0>);
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename A0, typename A1>
ClassBuilder<T>& ClassBuilder<T>::constructor2()
{
    addConstructor(new detail::ConstructorImpl2<T, A0, A1>);
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename A0, typename A1, typename A2>
ClassBuilder<T>& ClassBuilder<T>::constructor3()
{
    addConstructor(new detail::ConstructorImpl3<T, A0, A1, A2>);
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename A0, typename A1, typename A2, typename A3>
ClassBuilder<T>& ClassBuilder<T>::constructor4()
{
    addConstructor(new detail::ConstructorImpl4<T, A0, A1, A2, A3>);
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename A0, typename A1, typename A2, typename A3, typename A4>
ClassBuilder<T>& ClassBuilder<T>::constructor5()
{
    addConstructor(new detail::ConstructorImpl5<T, A0, A1, A2, A3, A4>);
    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <template <typename> class U>
ClassBuilder<T>& ClassBuilder<T>::external()
{
    // Create an instance of the mapper
    U<T> mapper;

    { // Retrieve the properties
        const std::size_t propertyCount = mapper.propertyCount();
        for (std::size_t i = 0; i < propertyCount; ++i)
        {
            addProperty(mapper.property(i));
        }
    }

    { // Retrieve the functions
        const std::size_t functionCount = mapper.functionCount();
        for (std::size_t i = 0; i < functionCount; ++i)
        {
            addFunction(mapper.function(i));
        }
    }

    // Done
    return *this;
}

} // namespace camp

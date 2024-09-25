#include "multiset.hpp"

// Hash functions

/**
 * @brief Computes a hash value for a MultiSet.
 *
 * This function iterates over each element of the MultiSet, hashes the element and its count,
 * and combines these hashes using XOR and shifting to produce a single hash value.
 *
 * @param ms The MultiSet to hash.
 * @return The computed hash value.
 */
std::size_t MultiSetHash::operator()(const MultiSet& ms) const
{
    std::size_t hash_value = 0;

    for (const auto& elem : ms.GetElements())
    {
        // Hash the element (variant) using VariantHash
        std::size_t element_hash = std::visit(VariantHash{}, elem.first);

        // Hash the count of the element
        std::size_t count_hash = std::hash<int>{}(elem.second);

        // Combine hashes (XOR with shifting)
        hash_value ^= element_hash ^ (count_hash << 1);
    }

    return hash_value;
}

/**
 * @brief Computes a hash value for a std::variant containing either a string or a shared_ptr to MultiSet.
 *
 * This function dispatches the hashing based on the type contained in the variant. If the variant holds
 * a string, it hashes it using std::hash. If it holds a shared_ptr to MultiSet, it hashes the MultiSet using
 * MultiSetHash.
 *
 * @param v The std::variant to hash.
 * @return The computed hash value.
 */
std::size_t VariantHash::operator()(const std::variant<std::string, std::shared_ptr<MultiSet>>& v) const
{
    return std::visit(
        [](const auto& value) -> std::size_t
        {
            using T = std::decay_t<decltype(value)>;  // Get the type of the variable and remove references and other
                                                      // type modifiers
            if constexpr (std::is_same_v<T, std::string>)
            {
                return std::hash<std::string>{}(value);
            }
            else
            {
                return MultiSetHash{}(*value);  // Custom hash for MultiSet
            }
        },
        v);
}

/**
 * @brief Checks for equality between two std::variant objects containing either a string or a shared_ptr to MultiSet.
 *
 * This function compares the values contained in the variants. If both variants hold a shared_ptr to MultiSet,
 * the comparison is made based on the content of the MultiSets rather than their addresses.
 *
 * @param lhs The left-hand side variant to compare.
 * @param rhs The right-hand side variant to compare.
 * @return True if the variants are equal, false otherwise.
 */
bool VariantEqual::operator()(const std::variant<std::string, std::shared_ptr<MultiSet>>& lhs,
                              const std::variant<std::string, std::shared_ptr<MultiSet>>& rhs) const
{
    return std::visit(
        [](const auto& left, const auto& right) -> bool
        {
            using LeftType = std::decay_t<decltype(left)>;
            using RightType = std::decay_t<decltype(right)>;
            if constexpr (std::is_same_v<LeftType, RightType>)
            {
                if constexpr (std::is_same_v<LeftType, std::shared_ptr<MultiSet>>)
                {
                    // For correct comparison, it is necessary to compare multisets by their content,
                    // not by their address, so we use dereferencing
                    return *left == *right;
                }
                else
                {
                    return left == right;
                }
            }
            // Different types (1 != {1})
            return false;
        },
        lhs, rhs);
}

// Implementations of MultiSet methods

/**
 * @brief Adds an element to the multiset. If the element already exists, its count is incremented.
 * @param element The element to be added to the multiset.
 */
void MultiSet::AddElement(const Element& element)
{
    auto it = elements_.find(element);

    if (it != elements_.end())
    {
        ++it->second;
    }
    else
    {
        elements_[element] = 1;
    }
}

/**
 * @brief Removes an element from the multiset. If the element's count reaches zero, it is removed from the multiset.
 * @param element The element to be removed from the multiset.
 * @throws std::runtime_error If the element does not exist in the multiset.
 */
void MultiSet::RemoveElement(const Element& element)
{
    auto it = elements_.find(element);

    if (it == elements_.end())
    {
        throw std::runtime_error("Element does not exist in the multiset");
    }

    if (--(it->second) == 0)
    {
        elements_.erase(it);
    }
}

/**
 * @brief Checks if the multiset contains a specific element.
 * @param element The element to check for presence in the multiset.
 * @return true if the element is in the multiset, false otherwise.
 */
bool MultiSet::IsContains(const Element& element) const
{
    auto it = elements_.find(element);
    return it != elements_.end();
}

/**
 * @brief Checks if the multiset is empty.
 * @return true if the multiset is empty, false otherwise.
 */
bool MultiSet::IsEmpty() const { return elements_.empty(); }

/**
 * @brief Returns the total number of elements in the multiset, counting duplicates.
 * @return The size of the multiset.
 */
size_t MultiSet::Size() const
{
    size_t res = 0;
    for (const auto& element : elements_)
    {
        res += element.second;
    }
    return res;
}

/**
 * @brief Builds a boolean multiset where each element is present with a count of 1.
 * @return A new MultiSet object representing the boolean version of the original multiset.
 */
MultiSet MultiSet::BuildBoolean() const
{
    MultiSet booleanSet;
    for (const auto& element : elements_)
    {
        booleanSet.elements_[element.first] = 1;
    }
    return booleanSet;
}

// Override operators

/**
 * @brief Compares two multisets for equality.
 * @param other The other multiset to compare with.
 * @return true if both multisets are equal, false otherwise.
 */
bool MultiSet::operator==(const MultiSet& other) const { return elements_ == other.elements_; }

/**
 * @brief Compares two multisets for inequality.
 * @param other The other multiset to compare with.
 * @return true if the multisets are not equal, false otherwise.
 */
bool MultiSet::operator!=(const MultiSet& other) const { return !(*this == other); }

/**
 * @brief Computes the union of two multisets.
 * @param other The other multiset to unite with.
 * @return A new MultiSet that is the union of the two multisets.
 */
MultiSet MultiSet::operator+(const MultiSet& other) const
{
    MultiSet result;
    result.elements_ = elements_;
    for (const auto& el : other.elements_)
    {
        const Element& element = el.first;
        int count_other = el.second;
        if (result.elements_.find(element) != result.elements_.end())
        {
            result.elements_[element] = std::max(result.elements_[element], count_other);
        }
        else
        {
            result.elements_[element] = count_other;
        }
    }
    return result;
}

/**
 * @brief Adds elements from another multiset to this multiset (union).
 * @param other The other multiset to add.
 * @return A reference to the updated multiset.
 */
MultiSet& MultiSet::operator+=(const MultiSet& other)
{
    for (const auto& el : other.elements_)
    {
        const Element& element = el.first;
        int count_other = el.second;
        if (elements_.find(element) != elements_.end())
        {
            elements_[element] = std::max(elements_[element], count_other);
        }
        else
        {
            elements_[element] = count_other;
        }
    }
    return *this;
}

/**
 * @brief Computes the intersection of two multisets.
 * @param other The other multiset to intersect with.
 * @return A new MultiSet that is the intersection of the two multisets.
 */
MultiSet MultiSet::operator*(const MultiSet& other) const
{
    MultiSet result;
    for (const auto& elem : elements_)
    {
        const Element& element = elem.first;
        int count_this = elem.second;
        auto it = other.elements_.find(element);
        if (it != other.elements_.end())
        {
            int count_other = it->second;
            result.elements_[element] = std::min(count_this, count_other);
        }
    }
    return result;
}

/**
 * @brief Updates this multiset by intersecting it with another multiset.
 * @param other The other multiset to intersect with.
 * @return A reference to the updated multiset.
 */
MultiSet& MultiSet::operator*=(const MultiSet& other)
{
    std::unordered_map<Element, int, VariantHash, VariantEqual> result;
    for (const auto& elem : elements_)
    {
        const Element& element = elem.first;
        int count_this = elem.second;
        auto it = other.elements_.find(element);
        if (it != other.elements_.end())
        {
            int count_other = it->second;
            result[element] = std::min(count_this, count_other);
        }
    }
    elements_ = std::move(result);
    return *this;
}

/**
 * @brief Computes the difference of two multisets (this - other).
 * @param other The other multiset to subtract.
 * @return A new MultiSet that represents the difference of the two multisets.
 */
MultiSet MultiSet::operator-(const MultiSet& other) const
{
    MultiSet result;
    for (const auto& el : elements_)
    {
        const Element& thisElement = el.first;
        const int& thisCount = el.second;
        auto it = other.elements_.find(thisElement);
        if (it != other.elements_.end())
        {
            const int& otherCount = it->second;
            if (thisCount > otherCount)
            {
                result.elements_[thisElement] = thisCount - otherCount;
            }
        }
        else
        {
            result.elements_[thisElement] = thisCount;
        }
    }
    for (const auto& el : other.elements_)
    {
        const Element& otherElement = el.first;
        if (elements_.find(otherElement) == elements_.end())
        {
            result.elements_[otherElement] = el.second;
        }
    }
    return result;
}

/**
 * @brief Updates this multiset by subtracting elements from another multiset.
 * @param other The other multiset to subtract.
 * @return A reference to the updated multiset.
 */
MultiSet& MultiSet::operator-=(const MultiSet& other)
{
    std::unordered_map<Element, int, VariantHash, VariantEqual> result;

    for (const auto& el : elements_)
    {
        const Element& thisElement = el.first;
        const int& thisCount = el.second;
        auto it = other.elements_.find(thisElement);
        if (it != other.elements_.end())
        {
            const int& otherCount = it->second;
            if (thisCount > otherCount)
            {
                result[thisElement] = thisCount - otherCount;
            }
        }
        else
        {
            result[thisElement] = thisCount;
        }
    }
    for (const auto& el : other.elements_)
    {
        const Element& otherElement = el.first;
        if (elements_.find(otherElement) == elements_.end())
        {
            result[otherElement] = el.second;
        }
    }
    elements_ = std::move(result);
    return *this;
}

// Input operator for MultiSet
/**
 * @brief Overloads the input stream operator for the MultiSet class.
 *
 * This operator reads a MultiSet from the input stream. The input
 * should start with '{' followed by elements separated by commas
 * and ending with '}'. Nested MultiSets can also be included.
 *
 * @param is The input stream to read from.
 * @param multiset The MultiSet instance to populate.
 * @return The modified input stream.
 */
std::istream& operator>>(std::istream& is, MultiSet& multiset)
{
    char ch;
    is >> ch;  // Set should start from '{'

    if (ch != '{')
    {
        is.setstate(std::ios::failbit);
        return is;
    }

    std::unordered_map<MultiSet::Element, int, VariantHash, VariantEqual> elements;

    while (true)
    {
        MultiSet::Element element;

        if (is.peek() == '{')  // Multiset case
        {
            auto nested_multiset = std::make_shared<MultiSet>();
            is >> *nested_multiset;
            element = nested_multiset;
        }
        else  // Element case
        {
            std::string str_element;

            is >> std::ws;  //

            while (is.peek() != ',' && is.peek() != '}' && is.peek() != std::char_traits<char>::eof())
            {
                char c;
                is.get(c);  // Get the separator or closing character
                str_element += c;
            }

            element = std::move(str_element);
        }

        elements[element]++;

        is >> std::ws;

        is >> ch;

        if (ch == '}')
        {
            break;
        }
        else if (ch != ',')
        {
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    multiset.SetElements(elements);
    return is;
}

// Output operator for std::variant
/**
 * @brief Overloads the output stream operator for std::variant.
 *
 * This operator writes a std::variant containing either a
 * std::string or a shared pointer to a MultiSet to the output stream.
 *
 * @param os The output stream to write to.
 * @param v The std::variant to output.
 * @return The modified output stream.
 */
std::ostream& operator<<(std::ostream& os, const std::variant<std::string, std::shared_ptr<MultiSet>>& v)
{
    std::visit(
        [&os](const auto& value)
        {
            using ValueType = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<ValueType, std::string>)
            {
                os << value;
            }
            else
            {
                os << *value;  // Use MultiSet's operator<<
            }
        },
        v);
    return os;
}

// Output operator for MultiSet
/**
 * @brief Overloads the output stream operator for the MultiSet class.
 *
 * This operator writes the contents of a MultiSet to the output stream,
 * formatted as a comma-separated list of elements enclosed in braces.
 *
 * @param os The output stream to write to.
 * @param multiset The MultiSet instance to output.
 * @return The modified output stream.
 */
std::ostream& operator<<(std::ostream& os, const MultiSet& multiset)
{
    os << "{";
    bool first = true;
    for (const auto& elem : multiset.GetElements())
    {
        for (int i = 0; i < elem.second; ++i)
        {
            if (!first)
            {
                os << ", ";
            }
            first = false;
            os << elem.first;  // This calls the variant operator<< if the element is a variant
        }
    }
    os << "}";
    return os;
}

/**
 * @brief Sets the elements of the MultiSet.
 *
 * This method populates the MultiSet with a given set of elements and
 * their counts.
 *
 * @param elements A map of elements and their respective counts to set.
 */
void MultiSet::SetElements(const std::unordered_map<Element, int, VariantHash, VariantEqual>& elements)
{
    elements_ = elements;
}

/**
 * @brief Retrieves the elements of the MultiSet.
 *
 * This method returns a constant reference to the internal map of
 * elements and their counts in the MultiSet.
 *
 * @return A constant reference to the unordered_map of elements and counts.
 */
const std::unordered_map<MultiSet::Element, int, VariantHash, VariantEqual>& MultiSet::GetElements() const
{
    return elements_;
}

#pragma once

#include <unordered_map>
#include <variant>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>

// Forward declaration of MultiSet
class MultiSet;

/**
 * @brief Hash functor for the MultiSet class.
 * 
 * This structure provides a way to generate a hash value for 
 * MultiSet objects, which is useful for using MultiSet in 
 * unordered containers.
 */
struct MultiSetHash {
    std::size_t operator()(const MultiSet& ms) const;
};

/**
 * @brief Hash functor for std::variant containing string or MultiSet.
 * 
 * This structure provides a way to generate a hash value for 
 * std::variant objects that can hold either a string or a 
 * shared pointer to a MultiSet.
 */
struct VariantHash {
    std::size_t operator()(const std::variant<std::string, std::shared_ptr<MultiSet>>& v) const;
};

/**
 * @brief Equality functor for std::variant containing string or MultiSet.
 * 
 * This structure provides a way to compare two std::variant 
 * objects for equality, handling both possible types.
 */
struct VariantEqual {
    bool operator()(const std::variant<std::string, std::shared_ptr<MultiSet>>& lhs,
                    const std::variant<std::string, std::shared_ptr<MultiSet>>& rhs) const;
};

/**
 * @brief Class representing a multiset of elements.
 * 
 * The MultiSet class allows for the storage and manipulation of 
 * a collection of elements, which can be either strings or 
 * nested MultiSets. It supports operations such as addition, 
 * removal, and various set operations (union, intersection, 
 * difference).
 */
class MultiSet
{
public:
    using Element = std::variant<std::string, std::shared_ptr<MultiSet>>;

    MultiSet() = default;

    /**
     * @brief Adds an element to the MultiSet.
     * 
     * This method increases the count of the specified element 
     * in the multiset.
     * 
     * @param element The element to add.
     */
    void AddElement(const Element &element);

    /**
     * @brief Removes an element from the MultiSet.
     * 
     * This method decreases the count of the specified element 
     * in the multiset, removing it completely if the count reaches zero.
     * 
     * @param element The element to remove.
     */
    void RemoveElement(const Element& element);

    /**
     * @brief Checks if the MultiSet contains a specific element.
     * 
     * @param element The element to check for.
     * @return True if the element is contained in the multiset, false otherwise.
     */
    bool IsContains(const Element& element) const;

    /**
     * @brief Checks if the MultiSet is empty.
     * 
     * @return True if the multiset has no elements, false otherwise.
     */
    bool IsEmpty() const;

    /**
     * @brief Gets the number of elements in the MultiSet.
     * 
     * @return The size of the multiset.
     */
    size_t Size() const;

    /**
     * @brief Builds a boolean representation of the MultiSet.
     * 
     * This method returns a MultiSet that represents the boolean 
     * logic of the current set.
     * 
     * @return A MultiSet representing the boolean logic.
     */
    MultiSet BuildBoolean() const;

    // Operators overload
    /**
     * @brief Checks for equality between two MultiSets.
     * 
     * @param other The other MultiSet to compare with.
     * @return True if the two MultiSets are equal, false otherwise.
     */
    bool operator==(const MultiSet& other) const;

    /**
     * @brief Checks for inequality between two MultiSets.
     * 
     * @param other The other MultiSet to compare with.
     * @return True if the two MultiSets are not equal, false otherwise.
     */
    bool operator!=(const MultiSet& other) const;

    /**
     * @brief Performs the union operation between two MultiSets.
     * 
     * @param other The other MultiSet to union with.
     * @return A new MultiSet representing the union of both.
     */
    MultiSet operator+(const MultiSet& other) const;

    /**
     * @brief Performs the union operation in place.
     * 
     * @param other The other MultiSet to union with.
     * @return A reference to this MultiSet after the union.
     */
    MultiSet& operator+=(const MultiSet& other);

    /**
     * @brief Performs the intersection operation between two MultiSets.
     * 
     * @param other The other MultiSet to intersect with.
     * @return A new MultiSet representing the intersection of both.
     */
    MultiSet operator*(const MultiSet& other) const;

    /**
     * @brief Performs the intersection operation in place.
     * 
     * @param other The other MultiSet to intersect with.
     * @return A reference to this MultiSet after the intersection.
     */
    MultiSet& operator*=(const MultiSet& other);

    /**
     * @brief Performs the difference operation between two MultiSets.
     * 
     * @param other The other MultiSet to subtract.
     * @return A new MultiSet representing the difference of both.
     */
    MultiSet operator-(const MultiSet& other) const;

    /**
     * @brief Performs the difference operation in place.
     * 
     * @param other The other MultiSet to subtract.
     * @return A reference to this MultiSet after the difference.
     */
    MultiSet& operator-=(const MultiSet& other);

    friend std::istream& operator>>(std::istream& is, MultiSet& multiset);
    friend std::ostream& operator<<(std::ostream& os, const MultiSet& multiset);

    /**
     * @brief Sets the elements of the MultiSet.
     * 
     * This method populates the MultiSet with a given set of elements 
     * and their counts.
     * 
     * @param elements A map of elements and their respective counts to set.
     */
    void SetElements(const std::unordered_map<Element, int, VariantHash, VariantEqual>& elements);

    /**
     * @brief Retrieves the elements of the MultiSet.
     * 
     * This method returns a constant reference to the internal map of 
     * elements and their counts in the MultiSet.
     * 
     * @return A constant reference to the unordered_map of elements and counts.
     */
    const std::unordered_map<Element, int, VariantHash, VariantEqual>& GetElements() const;

private:
    std::unordered_map<Element, int, VariantHash, VariantEqual> elements_;
};

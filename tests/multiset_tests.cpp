#include <gtest/gtest.h>

#include <sstream>

#include "multiset.hpp"

// MultiSet tests

TEST(MultiSetTest, AddElement)
{
    MultiSet ms;

    ms.AddElement("element1");
    EXPECT_TRUE(ms.IsContains("element1"));
    EXPECT_EQ(ms.Size(), 1);

    ms.AddElement("element1");
    EXPECT_EQ(ms.GetElements().at("element1"), 2);
}

TEST(MultiSetTest, RemoveElement)
{
    MultiSet ms;
    ms.AddElement("element1");

    ms.RemoveElement("element1");
    EXPECT_FALSE(ms.IsContains("element1"));
    EXPECT_EQ(ms.Size(), 0);

    EXPECT_THROW(ms.RemoveElement("element1"), std::runtime_error);
}

TEST(MultiSetTest, IsEmpty)
{
    MultiSet ms;

    EXPECT_TRUE(ms.IsEmpty());

    ms.AddElement("element1");
    EXPECT_FALSE(ms.IsEmpty());

    ms.RemoveElement("element1");
    EXPECT_TRUE(ms.IsEmpty());
}

TEST(MultiSetTest, BuildBoolean)
{
    MultiSet ms;
    ms.AddElement("element1");
    ms.AddElement("element1");

    MultiSet booleanSet = ms.BuildBoolean();
    EXPECT_TRUE(booleanSet.IsContains("element1"));
    EXPECT_EQ(booleanSet.GetElements().at("element1"), 1u);
    EXPECT_EQ(booleanSet.Size(), 1u);
}

TEST(MultiSetTest, EqualityOperator)
{
    MultiSet ms1;
    MultiSet ms2;

    EXPECT_TRUE(ms1 == ms2);

    ms1.AddElement("element1");
    ms2.AddElement("element1");
    EXPECT_TRUE(ms1 == ms2);

    ms2.AddElement("element2");
    EXPECT_FALSE(ms1 == ms2);
}

TEST(MultiSetTest, UnionOperator)
{
    MultiSet ms1;
    MultiSet ms2;

    ms1.AddElement("element1");
    ms2.AddElement("element2");
    MultiSet result = ms1 + ms2;
    EXPECT_TRUE(result.IsContains("element1"));
    EXPECT_TRUE(result.IsContains("element2"));
    EXPECT_EQ(result.Size(), 2);
}

TEST(MultiSetTest, IntersectionOperator)
{
    MultiSet ms1;
    MultiSet ms2;

    ms1.AddElement("element1");
    ms2.AddElement("element1");
    MultiSet result = ms1 * ms2;
    EXPECT_TRUE(result.IsContains("element1"));
    EXPECT_EQ(result.GetElements().at("element1"), 1);
    EXPECT_EQ(result.Size(), 1);
}

TEST(MultiSetTest, DifferenceOperator)
{
    MultiSet ms1;
    MultiSet ms2;

    ms1.AddElement("element1");
    ms2.AddElement("element1");
    MultiSet result = ms1 - ms2;
    EXPECT_TRUE(result.IsEmpty());

    ms1.AddElement("element2");
    result = ms1 - ms2;
    EXPECT_TRUE(result.IsContains("element2"));
    EXPECT_EQ(result.Size(), 1);
}

TEST(MultiSetTest, Equals_UnionOperation)
{
    MultiSet ms1;
    MultiSet ms2;

    ms1.AddElement("element1");
    ms2.AddElement("element2");

    ms1 += ms2;

    EXPECT_TRUE(ms1.IsContains("element1"));
    EXPECT_TRUE(ms1.IsContains("element2"));
    EXPECT_EQ(ms1.Size(), 2);
}

TEST(MultiSetTest, Equals_DifferenceOperation)
{
    MultiSet ms1;
    MultiSet ms2;

    ms1.AddElement("element1");
    ms1.AddElement("element2");
    ms2.AddElement("element1");

    ms1 -= ms2;

    EXPECT_TRUE(ms1.IsContains("element2"));
    EXPECT_EQ(ms1.Size(), 1);
    EXPECT_FALSE(ms1.IsContains("element1"));
}

TEST(MultiSetTest, Equals_IntersectionOperation)
{
    MultiSet ms1;
    MultiSet ms2;

    ms1.AddElement("element1");
    ms1.AddElement("element2");
    ms2.AddElement("element1");

    ms1 *= ms2;

    EXPECT_TRUE(ms1.IsContains("element1"));
    EXPECT_EQ(ms1.GetElements().at("element1"), 1);
    EXPECT_EQ(ms1.Size(), 1);
}

TEST(MultiSetTest, DuplicateElements)
{
    MultiSet ms;

    ms.AddElement("element1");
    ms.AddElement("element1");
    EXPECT_EQ(ms.GetElements().at("element1"), 2);
    EXPECT_EQ(ms.Size(), 2);
}

TEST(MultiSetTest, OutputOperator)
{
    MultiSet ms;
    ms.AddElement("element1");

    std::ostringstream oss;
    oss << ms;

    EXPECT_EQ(oss.str(), "{element1}");
}

// std::variant tests

TEST(VariantHashTest, HashString)
{
    std::variant<std::string, std::shared_ptr<MultiSet>> v = "test";
    VariantHash hasher;
    EXPECT_NE(hasher(v), 0);
}

TEST(VariantHashTest, HashMultiSet)
{
    std::shared_ptr<MultiSet> ms = std::make_shared<MultiSet>();
    ms->AddElement("element");
    std::variant<std::string, std::shared_ptr<MultiSet>> v = ms;
    VariantHash hasher;
    EXPECT_NE(hasher(v), 0);
}

TEST(VariantEqualTest, EqualityString)
{
    std::variant<std::string, std::shared_ptr<MultiSet>> v1 = "test";
    std::variant<std::string, std::shared_ptr<MultiSet>> v2 = "test";
    VariantEqual comparer;
    EXPECT_TRUE(comparer(v1, v2));
}

TEST(VariantEqualTest, EqualityMultiSet)
{
    std::shared_ptr<MultiSet> ms1 = std::make_shared<MultiSet>();
    std::shared_ptr<MultiSet> ms2 = std::make_shared<MultiSet>();
    ms1->AddElement("element");
    ms2->AddElement("element");
    std::variant<std::string, std::shared_ptr<MultiSet>> v1 = ms1;
    std::variant<std::string, std::shared_ptr<MultiSet>> v2 = ms2;
    VariantEqual comparer;
    EXPECT_TRUE(comparer(v1, v2));
}

// Exception checking

TEST(MultiSetTest, RemoveThrowsException)
{
    MultiSet ms;
    ms.AddElement("element1");

    EXPECT_NO_THROW(ms.RemoveElement("element1"));

    EXPECT_THROW(ms.RemoveElement("element2"), std::runtime_error);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// Nested MultiSet tests

TEST(MultiSetTest, AddNestedMultiSet)
{
    MultiSet ms1;
    MultiSet ms2;
    ms2.AddElement("nested_element");

    // Add a nested multiset to ms1
    ms1.AddElement(std::make_shared<MultiSet>(ms2));

    EXPECT_EQ(ms1.Size(), 1);

    // Check that the nested element is properly added
    auto nested_set = std::get<std::shared_ptr<MultiSet>>(ms1.GetElements().begin()->first);
    EXPECT_TRUE(nested_set->IsContains("nested_element"));
}

TEST(MultiSetTest, RemoveNestedMultiSet)
{
    MultiSet ms1;
    MultiSet ms2;
    ms2.AddElement("nested_element");

    // Add and then remove a nested multiset
    ms1.AddElement(std::make_shared<MultiSet>(ms2));
    EXPECT_EQ(ms1.Size(), 1);

    ms1.RemoveElement(std::make_shared<MultiSet>(ms2));
    EXPECT_EQ(ms1.Size(), 0);
}

TEST(MultiSetTest, UnionWithNestedMultiSets)
{
    MultiSet ms1;
    MultiSet ms2;
    MultiSet nested_ms1;
    MultiSet nested_ms2;

    nested_ms1.AddElement("nested_element1");
    nested_ms2.AddElement("nested_element2");

    ms1.AddElement(std::make_shared<MultiSet>(nested_ms1));
    ms2.AddElement(std::make_shared<MultiSet>(nested_ms2));

    MultiSet result = ms1 + ms2;

    // Ensure both nested multisets exist in the result
    EXPECT_EQ(result.Size(), 2);
    EXPECT_TRUE(result.IsContains(std::make_shared<MultiSet>(nested_ms1)));
    EXPECT_TRUE(result.IsContains(std::make_shared<MultiSet>(nested_ms2)));
}

TEST(MultiSetTest, IntersectionWithNestedMultiSets)
{
    MultiSet ms1;
    MultiSet ms2;
    MultiSet nested_ms;

    nested_ms.AddElement("shared_nested_element");

    ms1.AddElement(std::make_shared<MultiSet>(nested_ms));

    ms2.AddElement(std::make_shared<MultiSet>(nested_ms));

    MultiSet result = ms1 * ms2;

    // Ensure the shared nested multiset exists in the intersection
    EXPECT_EQ(result.Size(), 1);

    EXPECT_TRUE(result.IsContains(std::make_shared<MultiSet>(nested_ms)));
}

TEST(MultiSetTest, DifferenceWithNestedMultiSets)
{
    MultiSet ms1;
    MultiSet ms2;
    MultiSet nested_ms;

    nested_ms.AddElement("unique_nested_element");

    ms1.AddElement(std::make_shared<MultiSet>(nested_ms));

    // Difference between ms1 and ms2, where ms2 is empty
    MultiSet result = ms1 - ms2;

    // Ensure the nested multiset remains in the result
    EXPECT_EQ(result.Size(), 1);
    EXPECT_TRUE(result.IsContains(std::make_shared<MultiSet>(nested_ms)));
}

TEST(MultiSetTest, ComplexNestedMultiSet)
{
    MultiSet ms1;
    MultiSet nested_ms1;
    MultiSet nested_ms2;

    nested_ms1.AddElement("element_a");
    nested_ms1.AddElement("element_a");

    nested_ms2.AddElement("element_b");
    nested_ms2.AddElement("element_b");

    ms1.AddElement(std::make_shared<MultiSet>(nested_ms1));
    ms1.AddElement(std::make_shared<MultiSet>(nested_ms2));

    EXPECT_EQ(ms1.Size(), 2);

    bool contains_a = false;
    bool contains_b = false;

    for (const auto& elem : ms1.GetElements())
    {
        auto nested_set = std::get<std::shared_ptr<MultiSet>>(elem.first);
        if (nested_set->IsContains("element_a"))
        {
            contains_a = true;
            EXPECT_EQ(nested_set->GetElements().at("element_a"), 2);
        }
        if (nested_set->IsContains("element_b"))
        {
            contains_b = true;
            EXPECT_EQ(nested_set->GetElements().at("element_b"), 2);
        }
    }

    EXPECT_TRUE(contains_a);
    EXPECT_TRUE(contains_b);
}

TEST(MultiSetTest, AddNestedMultiSetWithDifferentOrder)
{
    MultiSet ms1;
    MultiSet nested_ms;

    nested_ms.AddElement("element_1");
    nested_ms.AddElement("element_2");
    nested_ms.AddElement("element_3");

    ms1.AddElement(std::make_shared<MultiSet>(nested_ms));

    EXPECT_EQ(ms1.Size(), 1);

    MultiSet nested_ms2;
    nested_ms2.AddElement("element_3");
    nested_ms2.AddElement("element_1");
    nested_ms2.AddElement("element_2");

    ms1.AddElement(std::make_shared<MultiSet>(nested_ms2));

    // Cardinality of the set is sum of elements repeats
    EXPECT_EQ(ms1.Size(), 2);
    // Number of elements of the set must be 1, cause it`s identical elements
    EXPECT_EQ(ms1.GetElements().size(), 1);

    // Access the only element in ms1 and check it contains "element_1", "element_2", "element_3"
    auto nested_set = std::get<std::shared_ptr<MultiSet>>(ms1.GetElements().begin()->first);

    EXPECT_TRUE(nested_set->IsContains("element_1"));
    EXPECT_TRUE(nested_set->IsContains("element_2"));
    EXPECT_TRUE(nested_set->IsContains("element_3"));
}

TEST(MultiSetTest, InputOperatorWithNestedMultiSet)
{
    MultiSet ms;

    std::istringstream input("{{nested_element1, nested_element2}, nested_element3}");
    input >> ms;

    EXPECT_EQ(ms.Size(), 2);

    MultiSet first_el;
    MultiSet second_el;

    std::istringstream first_input("{nested_element1, nested_element2}");
    first_input >> first_el;

    std::istringstream second_input("nested_element3");
    second_input >> second_el;

    EXPECT_TRUE(ms.IsContains(std::make_shared<MultiSet>(first_el)));
}

TEST(MultiSetTest, CompareMultiSetWithElementAndNestedSet)
{
    MultiSet ms1;
    ms1.AddElement("1");

    MultiSet nested_ms;
    nested_ms.AddElement("1");

    MultiSet ms2;
    ms2.AddElement(std::make_shared<MultiSet>(nested_ms));

    EXPECT_NE(ms1, ms2);

    EXPECT_EQ(ms1.Size(), 1);
    EXPECT_TRUE(ms1.IsContains("1"));

    EXPECT_EQ(ms2.Size(), 1);
    auto nested_set = std::get<std::shared_ptr<MultiSet>>(ms2.GetElements().begin()->first);
    EXPECT_TRUE(nested_set->IsContains("1"));
}

TEST(MultiSetTest, InequalityOperatorTest)
{
    MultiSet ms1;
    ms1.AddElement("1");

    MultiSet ms2;
    ms2.AddElement("2");

    EXPECT_TRUE(ms1 != ms2);
}
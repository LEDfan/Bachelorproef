#include <gtest/gtest.h>
#include <pool/ContactPool.h>
#include <pop/Person.h>

using namespace stride;

TEST(ContactPool, BasicRemoveExpatTest)
{
        ContactPool cp;
        Person      person1;
        Person      person2;
        Person      person3;
        Person      person4;
        Person      person5;

        cp.AddMember(&person1);
        cp.AddMember(&person2);

        // add person3 as expat
        cp.AddExpat(&person3);

        // add person4 after person3
        cp.AddExpat(&person4);

        // add person5 after person4
        cp.AddExpat(&person5);

        EXPECT_EQ(&person1, cp.GetMember(0));
        EXPECT_EQ(&person2, cp.GetMember(1));
        EXPECT_EQ(&person3, cp.GetMember(2));
        EXPECT_EQ(&person4, cp.GetMember(3));
        EXPECT_EQ(&person5, cp.GetMember(4));

        cp.RemoveExpat(&person3);

        EXPECT_EQ(&person1, cp.GetMember(0));
        EXPECT_EQ(&person2, cp.GetMember(1));
        EXPECT_EQ(&person4, cp.GetMember(2));
        EXPECT_EQ(&person5, cp.GetMember(3));

        // person5 is another expat, remove should still work (i.e. the index has been updated)
        cp.RemoveExpat(&person5);

        EXPECT_EQ(&person1, cp.GetMember(0));
        EXPECT_EQ(&person2, cp.GetMember(1));
        EXPECT_EQ(&person4, cp.GetMember(2));
}

TEST(ContactPool, BasicRemoveMember)
{
        ContactPool cp;
        Person      person1;
        Person      person2;
        Person      person3;
        Person      person4;
        Person      person5;

        cp.AddMember(&person1);
        cp.AddMember(&person2);
        cp.AddMember(&person3);
        cp.AddExpat(&person4);
        cp.AddExpat(&person5);

        EXPECT_EQ(&person1, cp.GetMember(0));
        EXPECT_EQ(&person2, cp.GetMember(1));
        EXPECT_EQ(&person3, cp.GetMember(2));
        EXPECT_EQ(&person4, cp.GetMember(3));
        EXPECT_EQ(&person5, cp.GetMember(4));

        cp.RemoveMember(&person1);

        EXPECT_EQ(&person2, cp.GetMember(0));
        EXPECT_EQ(&person3, cp.GetMember(1));
        EXPECT_EQ(&person4, cp.GetMember(2));
        EXPECT_EQ(&person5, cp.GetMember(3));

        cp.RemoveExpat(&person4);

        EXPECT_EQ(&person2, cp.GetMember(0));
        EXPECT_EQ(&person3, cp.GetMember(1));
        EXPECT_EQ(&person5, cp.GetMember(2));

        cp.RemoveExpat(&person5);

        EXPECT_EQ(&person2, cp.GetMember(0));
        EXPECT_EQ(&person3, cp.GetMember(1));
}

TEST(ContactPool, BasicSortTest)
{
        ContactPool cp;
        Person      person1;
        Person      person2;
        Person      person3;
        Person      person4;
        Person      person5;

        person4.SetTravelling(true);
        person5.SetTravelling(true);

        person1.GetHealth().SetImmune();
        person3.GetHealth().SetImmune();
        person5.GetHealth().SetImmune();
        person2.GetHealth().SetSusceptible();
        person4.GetHealth().SetSusceptible();

        cp.AddMember(&person1);
        cp.AddMember(&person2);
        cp.AddMember(&person3);
        cp.AddExpat(&person4);
        cp.AddExpat(&person5);

        EXPECT_EQ(&person1, cp.GetMember(0));
        EXPECT_EQ(&person2, cp.GetMember(1));
        EXPECT_EQ(&person3, cp.GetMember(2));
        EXPECT_EQ(&person4, cp.GetMember(3));
        EXPECT_EQ(&person5, cp.GetMember(4));

        cp.SortMembers();

        // Susceptible
        EXPECT_EQ(&person4, cp.GetMember(0));
        EXPECT_EQ(&person2, cp.GetMember(1));

        // Immune
        EXPECT_EQ(&person3, cp.GetMember(2));
        EXPECT_EQ(&person1, cp.GetMember(3));
        EXPECT_EQ(&person5, cp.GetMember(4));

        cp.RemoveExpat(&person4);

        // Susceptible
        EXPECT_EQ(&person2, cp.GetMember(0));

        // Immune
        EXPECT_EQ(&person3, cp.GetMember(1));
        EXPECT_EQ(&person1, cp.GetMember(2));
        EXPECT_EQ(&person5, cp.GetMember(3));

        cp.RemoveExpat(&person5);

        // Susceptible
        EXPECT_EQ(&person2, cp.GetMember(0));

        // Immune
        EXPECT_EQ(&person3, cp.GetMember(1));
        EXPECT_EQ(&person1, cp.GetMember(2));
}

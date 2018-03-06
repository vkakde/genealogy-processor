#include "stdafx.h"
#include "Gedcom.h"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(US02_TC01)
{
	Gedcom::gedcom gedcomInstance("testFileError.ged");

	BOOST_TEST_MESSAGE("\nTesting US02 ...\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US02(), true);
}

BOOST_AUTO_TEST_CASE(US08_TC01)
{
	Gedcom::gedcom gedcomInstance("testFileError.ged");

	BOOST_TEST_MESSAGE("\nTesting US08 ...\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US08(), true);
}

BOOST_AUTO_TEST_CASE(US09_TC01)
{
	Gedcom::gedcom gedcomInstance("testFileError.ged");

	BOOST_TEST_MESSAGE("\nTesting US09 ...\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US09(), true);
}

BOOST_AUTO_TEST_CASE(US16_TC01)
{
	Gedcom::gedcom gedcomInstance("testFileError.ged");

	BOOST_TEST_MESSAGE("\nTesting US16 ...\n");
	BOOST_CHECK_EQUAL(gedcomInstance.US16(), true);
}

// EOF
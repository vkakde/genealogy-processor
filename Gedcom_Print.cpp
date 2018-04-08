#include "stdafx.h"
#include "Gedcom.h"

void Gedcom::gedcom::printGedcomData() {
	std::cout << gedcomData << "\n";
}

void Gedcom::gedcom::printSupportedTags() {
	for (auto& i : supportedTagsAndLevels) {
		std::cout << "\n" << i.first << "  " << i.second;
	}
}

//\author mjosephs
//For US 27 - Printing the age of each individual
std::string getAge(std::string birthday) {
	boost::gregorian::date today = boost::gregorian::day_clock::local_day();
	boost::gregorian::date birth = boost::gregorian::from_uk_string(birthday);
	boost::gregorian::date_duration age = today - birth;
	std::string age_str = std::to_string(age.days()/365);

	return age_str;
}

void Gedcom::gedcom::printIndividualTable() {
	std::cout << "\n" << "Individuals" << std::endl;

	std::cout << "----------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "      ID       |" << "|            Name            |" << "|   Gender   |" << "|      Birthday      |" << "|     Death Date     |" << "|     Age (US 27)    " <<std::endl;
	std::cout << "----------------------------------------------------------------------------------------------------------------------------" << std::endl;

	for (int i = 0; i < individualList.size(); ++i) {
		std::cout << "     " << std::setw(10) << std::left << individualList[i].id 
			<< "||    " << std::setw(20) << std::left << individualList[i].name << "    "
			<< "||     " << std::setw(5) << std::left << individualList[i].sex << "  "
			<< "||     " << std::setw(15) << std::left << individualList[i].birthDay
			<< "||     " << std::setw(15) << std::left << individualList[i].deathDate 
			<< "||     " << std::setw(15) << std::left << getAge(individualList[i].birthDay) << std::endl;
	}
	std::cout << "----------------------------------------------------------------------------------------------------------------------------" << std::endl;
}

void Gedcom::gedcom::printFamilyTable() {
	std::string husb_name;
	std::string wife_name;

	std::cout << "\n" << "Families" << std::endl;

	std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "      ID       |" << "|   Husband ID  |" << "|      Husband Name      |" << "|    Wife ID    |" << "|       Wife Name        |" << "|     Marriage Date      |" << "|      Divorce Date      |" << "|     Children IDs     " << std::endl;
	std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

	for (int i = 0; i < familyList.size(); ++i) {
		std::string childrenIds = "";

		for (int j = 0; j < familyList[i].childrenIds.size(); j++) {
			std::string childId = familyList[i].childrenIds[j];
			if (j == familyList[i].childrenIds.size() - 1) {
				childrenIds += childId;
			}
			else {
				childrenIds += childId + ", ";
			}
		}
		std::cout << "     " << std::setw(10) << std::left << familyList[i].id
			<< "||     " << std::setw(10) << std::left << familyList[i].husbandId
			<< "||    " << std::setw(20) << std::left << familyList[i].husband.name
			<< "||     " << std::setw(10) << std::left << familyList[i].wifeId
			<< "||    " << std::setw(20) << std::left << familyList[i].wife.name
			<< "||    " << std::setw(20) << std::left << familyList[i].marriageDate
			<< "||    " << std::setw(20) << std::left << familyList[i].divorceDate
			<< "||    " << childrenIds <<std::endl;
	}
	std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
}

///\author gmccorma
// List siblings in families by decreasing age, i.e. oldest siblings first
void Gedcom::gedcom::US28() {
	//const int count = 0;
	std::vector<std::string> childIds;
	
	for (auto it_family : familyList) {
		// get dates of birth of each child
		//std::cout << "Siblings in family " << it_family.id << ": " << it_family.childrenIds << std::endl;
		for (std::vector<std::string>::const_iterator i = it_family.childrenIds.begin(); i != it_family.childrenIds.end(); ++i) {
			for (auto it_individual : individualList) {
				// find child, put into vector list
				if (it_individual.id.compare(*i) == 0) {
					childIds.push_back(it_individual.birthDay);
					// count number of children in family
					for (auto elem : it_family.childrenIds) {
						const int count = std::count(it_family.childrenIds.begin(), it_family.childrenIds.end(), elem);
					}
					//std::string arr[count] = {};
					std::cout << "Siblings in family " << it_family.id << ": " << it_individual.id << std::endl;
				}
			}
		}

	}
}

///\author gmccorma
// List all deceased individuals in a GEDCOM file
void Gedcom::gedcom::US29() {
	for (auto it_individual : individualList) {
		if (it_individual.deathDate != "") {
			std::cout << "Deceased individual with ID: " << it_individual.id << std::endl;
		}
	}
}

///\author LouisRH
// List living married
void Gedcom::gedcom::US30() {
	for (auto it_family : familyList) {
		if (it_family.divorceDate == "" && it_family.husband.deathDate == "" && it_family.wife.deathDate == "") {
			std::cout << "Living married individual with ID: " << it_family.husbandId << std::endl;
			std::cout << "Living married individual with ID: " << it_family.wifeId << std::endl;
		}
	}
}

// Helper for US31
bool findFamily(std::vector<Family::Family> familyList, std::string id) {
	for (auto it_family : familyList) {
		if (it_family.husbandId == id || it_family.wifeId == id) {
			return false;
		}
	}

	return true;
}

// Helper for US31
int stringToIntNew(std::string str) {
	int num;
	std::stringstream stream(str);
	stream >> num;
	return num;
}

///\author LouisRH
// List living single
void Gedcom::gedcom::US31() {
	for (auto it_individual : individualList) {
		if (it_individual.deathDate == "" && stringToIntNew(getAge(it_individual.birthDay)) >= 30) {
			if (findFamily(familyList, it_individual.id)) {
				std::cout << "Living single individual over 30 with ID: " << it_individual.id << std::endl;
			}
		}
	}
}

///\author vkakde
///\brief Multiple births
void Gedcom::gedcom::US32() {
	std::set<int> multipleBirthIndividuals;
	for (int i = 0; i < familyList.size(); i++) {
		if (familyList[i].children.size() != 0) {
			for (int j = 0; j < familyList[i].children.size(); j++) {
				for (int k = 0; k < familyList[i].children.size(); k++) {
					if (j != k && familyList[i].children[j].birthDay == familyList[i].children[k].birthDay) {
						std::cout << "\nMultiple births in family " << familyList[i].id << " for children " << individualList[j].id << " and " << individualList[k].id;
					}
				}
			}
		}
	}
}
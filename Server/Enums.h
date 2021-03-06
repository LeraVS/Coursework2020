#pragma once

enum class TransportType {
	ONFOOT = 1, CAR, BUS, TRUCK  
};

enum class TransportedCargoType {
	 PERISHABLEEDIBLE, EDIBLE, MANUFACTUREDGOODS, PEOPLE 
};

enum class DocumentType {
	PASSPORT = 1, TECHPASSPORT, DRIVERSLICENSE, CMRINSURANCE, INVOICE, PACKINGLIST, CERTIFICATEOFORIGIN, TRANSITDECLARATION
};

enum class RequestToQueueType {
	NOTSEND = 1, NOTREVIEWED, REJECTED, ACCEPTED
};
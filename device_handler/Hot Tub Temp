/**
 *  Read Temp From Arduino
 *
 *  Original Author: urman
 *  Date: 2013-03-14
 *  Revision 2014-07-10
 *  Modifications made by Jeremy Koerber on 04-13-17
 */
 // for the UI

metadata {

	definition (name: "Hot Tub Temp", author: "jeremykoerber") {
    		capability "refresh"
	}
  // simulator metadata
	simulator {
		for (int i = 0; i <= 100; i += 5) {
        	def str = i.toString().collect{new BigInteger(new Integer((byte)it).toString()).toString(16)}.join('')
			status "${i}": "catchall: 0104 0000 01 01 0040 00 0A21 00 00 0000 0A 00 0A${str}2E30"
		}
	}

	// UI tile definitions
	tiles(scale: 2) {
		valueTile("temperature", "device.temperature", width: 6, height: 6) {
			state "val", label:'${currentValue}º', unit:"dF", icon: "st.Weather.weather2", defaultState: true, backgroundColors:[
                [value: 90, color: "#00D2FF"],
                [value: 100, color: "#00D2FF"],
                [value: 101, color: "#79FE33"],
                [value: 104, color: "#79FE33"],
                [value: 105, color: "#F86626"],
                [value: 109, color: "#D43927"]]
		}
		main "temperature"
	}
}

// Parse incoming device messages to generate events
Map parse(String description) {
	log.trace description
	def result = createEvent(name: "temperature", value: zigbee.parse(description)?.text)
    log.debug result?.descriptionText
    return result
}

def installed() {
    sendEvent(name: "temperature", value: zigbee.parse(description)?.text)
}
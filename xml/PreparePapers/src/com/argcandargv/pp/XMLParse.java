package com.argcandargv.pp;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Unmarshaller;

import com.argcandargv.pp.entity.Proceeding;

/**
 * 
 * 
 * @author yu
 * 
 */
public class XMLParse {
	String bpkg = "com.argcandargv.pp.entity";
	Proceeding proc; // root element

	public XMLData parse(String fileName) throws JAXBException  {
		XMLData xmlData = new XMLData();
		xmlData.setFileName(fileName);
		JAXBContext jAXBContext = JAXBContext.newInstance(bpkg);
		Unmarshaller unmarshaller = jAXBContext.createUnmarshaller();
		proc = (Proceeding) unmarshaller.unmarshal(new java.io.File(fileName));
		xmlData.setProceeding(proc);
		return xmlData;
	}
}

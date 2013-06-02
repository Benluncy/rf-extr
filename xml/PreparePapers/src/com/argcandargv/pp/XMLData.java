package com.argcandargv.pp;

import com.argcandargv.pp.entity.Proceeding;

/**
 * 
 * @author yu
 * 
 */
public class XMLData {
	Proceeding proceeding;
	String fileName;
	
	XMLData(){
		proceeding = new Proceeding();
	}

	public Proceeding getProceeding() {
		return proceeding;
	}
	public void setProceeding(Proceeding proceeding) {
		this.proceeding = proceeding;
	}
	public String getFileName() {
		return fileName;
	}
	public void setFileName(String fileName) {
		this.fileName = fileName;
	}
}

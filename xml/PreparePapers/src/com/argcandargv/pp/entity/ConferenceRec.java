package com.argcandargv.pp.entity;


import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;


/**
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "", propOrder = {
    "conferenceDate",
    "conferenceLoc",
    "conferenceUrl"
})
@XmlRootElement(name = "conference_rec")
public class ConferenceRec {

    @XmlElement(name = "conference_date")
    protected ConferenceDate conferenceDate;
    @XmlElement(name = "conference_loc")
    protected ConferenceLoc conferenceLoc;
    @XmlElement(name = "conference_url")
    protected String conferenceUrl;

    /**
     * Gets the value of the conferenceDate property.
     * 
     * @return
     *     possible object is
     *     {@link ConferenceDate }
     *     
     */
    public ConferenceDate getConferenceDate() {
        return conferenceDate;
    }

    /**
     * Sets the value of the conferenceDate property.
     * 
     * @param value
     *     allowed object is
     *     {@link ConferenceDate }
     *     
     */
    public void setConferenceDate(ConferenceDate value) {
        this.conferenceDate = value;
    }

    /**
     * Gets the value of the conferenceLoc property.
     * 
     * @return
     *     possible object is
     *     {@link ConferenceLoc }
     *     
     */
    public ConferenceLoc getConferenceLoc() {
        return conferenceLoc;
    }

    /**
     * Sets the value of the conferenceLoc property.
     * 
     * @param value
     *     allowed object is
     *     {@link ConferenceLoc }
     *     
     */
    public void setConferenceLoc(ConferenceLoc value) {
        this.conferenceLoc = value;
    }

    /**
     * Gets the value of the conferenceUrl property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getConferenceUrl() {
        return conferenceUrl;
    }

    /**
     * Sets the value of the conferenceUrl property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setConferenceUrl(String value) {
        this.conferenceUrl = value;
    }

}

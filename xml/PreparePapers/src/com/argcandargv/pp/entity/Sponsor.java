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
    "sponsorId",
    "sponsorName",
    "sponsorAbbr"
})
@XmlRootElement(name = "sponsor")
public class Sponsor {

    @XmlElement(name = "sponsor_id")
    protected String sponsorId;
    @XmlElement(name = "sponsor_name", required = true)
    protected String sponsorName;
    @XmlElement(name = "sponsor_abbr")
    protected String sponsorAbbr;

    /**
     * Gets the value of the sponsorId property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSponsorId() {
        return sponsorId;
    }

    /**
     * Sets the value of the sponsorId property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSponsorId(String value) {
        this.sponsorId = value;
    }

    /**
     * Gets the value of the sponsorName property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSponsorName() {
        return sponsorName;
    }

    /**
     * Sets the value of the sponsorName property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSponsorName(String value) {
        this.sponsorName = value;
    }

    /**
     * Gets the value of the sponsorAbbr property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSponsorAbbr() {
        return sponsorAbbr;
    }

    /**
     * Sets the value of the sponsorAbbr property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSponsorAbbr(String value) {
        this.sponsorAbbr = value;
    }

}

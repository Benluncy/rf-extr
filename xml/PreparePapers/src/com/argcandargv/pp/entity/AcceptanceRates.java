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
    "totalSubmitted",
    "totalAccepted"
})
@XmlRootElement(name = "acceptance_rates")
public class AcceptanceRates {

    @XmlElement(name = "total_submitted", required = true)
    protected String totalSubmitted;
    @XmlElement(name = "total_accepted", required = true)
    protected String totalAccepted;

    /**
     * Gets the value of the totalSubmitted property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getTotalSubmitted() {
        return totalSubmitted;
    }

    /**
     * Sets the value of the totalSubmitted property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setTotalSubmitted(String value) {
        this.totalSubmitted = value;
    }

    /**
     * Gets the value of the totalAccepted property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getTotalAccepted() {
        return totalAccepted;
    }

    /**
     * Sets the value of the totalAccepted property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setTotalAccepted(String value) {
        this.totalAccepted = value;
    }

}

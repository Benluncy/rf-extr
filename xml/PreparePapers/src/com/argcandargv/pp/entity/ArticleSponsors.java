package com.argcandargv.pp.entity;

import java.util.ArrayList;
import java.util.List;
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
    "fundingAgency",
    "grantNumbers"
})
@XmlRootElement(name = "article_sponsors")
public class ArticleSponsors {

    @XmlElement(name = "funding_agency", required = true)
    protected String fundingAgency;
    @XmlElement(name = "grant_numbers")
    protected List<GrantNumbers> grantNumbers;

    /**
     * Gets the value of the fundingAgency property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getFundingAgency() {
        return fundingAgency;
    }

    /**
     * Sets the value of the fundingAgency property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setFundingAgency(String value) {
        this.fundingAgency = value;
    }

    /**
     * Gets the value of the grantNumbers property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the grantNumbers property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getGrantNumbers().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link GrantNumbers }
     * 
     * 
     */
    public List<GrantNumbers> getGrantNumbers() {
        if (grantNumbers == null) {
            grantNumbers = new ArrayList<GrantNumbers>();
        }
        return this.grantNumbers;
    }

}

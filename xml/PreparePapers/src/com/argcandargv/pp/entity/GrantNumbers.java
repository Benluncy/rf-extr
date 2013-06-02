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
    "grantNumber"
})
@XmlRootElement(name = "grant_numbers")
public class GrantNumbers {

    @XmlElement(name = "grant_number", required = true)
    protected List<GrantNumber> grantNumber;

    /**
     * Gets the value of the grantNumber property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the grantNumber property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getGrantNumber().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link GrantNumber }
     * 
     * 
     */
    public List<GrantNumber> getGrantNumber() {
        if (grantNumber == null) {
            grantNumber = new ArrayList<GrantNumber>();
        }
        return this.grantNumber;
    }

}

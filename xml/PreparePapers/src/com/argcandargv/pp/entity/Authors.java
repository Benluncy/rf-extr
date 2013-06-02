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
    "au"
})
@XmlRootElement(name = "authors")
public class Authors {

    @XmlElement(required = true)
    protected List<Au> au;

    /**
     * Gets the value of the au property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the au property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getAu().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link Au }
     * 
     * 
     */
    public List<Au> getAu() {
        if (au == null) {
            au = new ArrayList<Au>();
        }
        return this.au;
    }

}

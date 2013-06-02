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
    "incoop"
})
@XmlRootElement(name = "incoop_rec")
public class IncoopRec {

    @XmlElement(required = true)
    protected List<Incoop> incoop;

    /**
     * Gets the value of the incoop property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the incoop property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getIncoop().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link Incoop }
     * 
     * 
     */
    public List<Incoop> getIncoop() {
        if (incoop == null) {
            incoop = new ArrayList<Incoop>();
        }
        return this.incoop;
    }

}

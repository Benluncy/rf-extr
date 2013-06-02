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
    "copyrightHolder"
})
@XmlRootElement(name = "ccc")
public class Ccc {

    @XmlElement(name = "copyright_holder", required = true)
    protected List<CopyrightHolder> copyrightHolder;

    /**
     * Gets the value of the copyrightHolder property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the copyrightHolder property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getCopyrightHolder().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link CopyrightHolder }
     * 
     * 
     */
    public List<CopyrightHolder> getCopyrightHolder() {
        if (copyrightHolder == null) {
            copyrightHolder = new ArrayList<CopyrightHolder>();
        }
        return this.copyrightHolder;
    }

}

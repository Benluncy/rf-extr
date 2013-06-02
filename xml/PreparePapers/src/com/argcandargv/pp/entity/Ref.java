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
    "refObjId",
    "refObjPid",
    "refSeqNo",
    "refText",
    "refId"
})
@XmlRootElement(name = "ref")
public class Ref {

    @XmlElement(name = "ref_obj_id")
    protected String refObjId;
    @XmlElement(name = "ref_obj_pid")
    protected String refObjPid;
    @XmlElement(name = "ref_seq_no", required = true)
    protected String refSeqNo;
    @XmlElement(name = "ref_text", required = true)
    protected String refText;
    @XmlElement(name = "ref_id")
    protected String refId;

    /**
     * Gets the value of the refObjId property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getRefObjId() {
        return refObjId;
    }

    /**
     * Sets the value of the refObjId property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setRefObjId(String value) {
        this.refObjId = value;
    }

    /**
     * Gets the value of the refObjPid property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getRefObjPid() {
        return refObjPid;
    }

    /**
     * Sets the value of the refObjPid property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setRefObjPid(String value) {
        this.refObjPid = value;
    }

    /**
     * Gets the value of the refSeqNo property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getRefSeqNo() {
        return refSeqNo;
    }

    /**
     * Sets the value of the refSeqNo property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setRefSeqNo(String value) {
        this.refSeqNo = value;
    }

    /**
     * Gets the value of the refText property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getRefText() {
        return refText;
    }

    /**
     * Sets the value of the refText property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setRefText(String value) {
        this.refText = value;
    }

    /**
     * Gets the value of the refId property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getRefId() {
        return refId;
    }

    /**
     * Sets the value of the refId property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setRefId(String value) {
        this.refId = value;
    }

}

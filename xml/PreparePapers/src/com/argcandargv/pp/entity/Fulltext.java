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
    "file",
    "articleImage",
    "ftBody"
})
@XmlRootElement(name = "fulltext")
public class Fulltext {

    @XmlElement(required = true)
    protected List<File> file;
    @XmlElement(name = "article_image")
    protected ArticleImage articleImage;
    @XmlElement(name = "ft_body")
    protected String ftBody;

    /**
     * Gets the value of the file property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the file property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getFile().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link File }
     * 
     * 
     */
    public List<File> getFile() {
        if (file == null) {
            file = new ArrayList<File>();
        }
        return this.file;
    }

    /**
     * Gets the value of the articleImage property.
     * 
     * @return
     *     possible object is
     *     {@link ArticleImage }
     *     
     */
    public ArticleImage getArticleImage() {
        return articleImage;
    }

    /**
     * Sets the value of the articleImage property.
     * 
     * @param value
     *     allowed object is
     *     {@link ArticleImage }
     *     
     */
    public void setArticleImage(ArticleImage value) {
        this.articleImage = value;
    }

    /**
     * Gets the value of the ftBody property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getFtBody() {
        return ftBody;
    }

    /**
     * Sets the value of the ftBody property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setFtBody(String value) {
        this.ftBody = value;
    }

}

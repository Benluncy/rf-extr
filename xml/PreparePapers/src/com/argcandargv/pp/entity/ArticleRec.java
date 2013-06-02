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
    "articleId",
    "sortKey",
    "displayLabel",
    "pages",
    "displayNo",
    "articlePublicationDate",
    "seqNo",
    "title",
    "subtitle",
    "pageFrom",
    "pageTo",
    "doiNumber",
    "url",
    "foreignTitle",
    "foreignSubtitle",
    "language",
    "_abstract",
    "keywords",
    "categories",
    "generalTerms",
    "authors",
    "references",
    "fulltext",
    "supplements",
    "qualifiers",
    "articleType",
    "bestPaperText",
    "articleSponsors",
    "ccc",
    "printOnDemand"
})
@XmlRootElement(name = "article_rec")
public class ArticleRec {

    @XmlElement(name = "article_id")
    protected String articleId;
    @XmlElement(name = "sort_key", required = true)
    protected String sortKey;
    @XmlElement(name = "display_label", required = true)
    protected String displayLabel;
    protected String pages;
    @XmlElement(name = "display_no")
    protected String displayNo;
    @XmlElement(name = "article_publication_date", required = true)
    protected String articlePublicationDate;
    @XmlElement(name = "seq_no", required = true)
    protected String seqNo;
    @XmlElement(required = true)
    protected String title;
    protected String subtitle;
    @XmlElement(name = "page_from")
    protected String pageFrom;
    @XmlElement(name = "page_to")
    protected String pageTo;
    @XmlElement(name = "doi_number")
    protected String doiNumber;
    protected String url;
    @XmlElement(name = "foreign_title")
    protected String foreignTitle;
    @XmlElement(name = "foreign_subtitle")
    protected String foreignSubtitle;
    protected String language;
    @XmlElement(name = "abstract")
    protected Abstract _abstract;
    protected Keywords keywords;
    @XmlElement(required = true)
    protected Categories categories;
    @XmlElement(name = "general_terms")
    protected GeneralTerms generalTerms;
    @XmlElement(required = true)
    protected Authors authors;
    protected References references;
    protected Fulltext fulltext;
    protected Supplements supplements;
    protected Qualifiers qualifiers;
    @XmlElement(name = "article_type")
    protected ArticleType articleType;
    @XmlElement(name = "best_paper_text")
    protected String bestPaperText;
    @XmlElement(name = "article_sponsors")
    protected List<ArticleSponsors> articleSponsors;
    protected Ccc ccc;
    @XmlElement(name = "print_on_demand")
    protected PrintOnDemand printOnDemand;

    /**
     * Gets the value of the articleId property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getArticleId() {
        return articleId;
    }

    /**
     * Sets the value of the articleId property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setArticleId(String value) {
        this.articleId = value;
    }

    /**
     * Gets the value of the sortKey property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSortKey() {
        return sortKey;
    }

    /**
     * Sets the value of the sortKey property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSortKey(String value) {
        this.sortKey = value;
    }

    /**
     * Gets the value of the displayLabel property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getDisplayLabel() {
        return displayLabel;
    }

    /**
     * Sets the value of the displayLabel property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setDisplayLabel(String value) {
        this.displayLabel = value;
    }

    /**
     * Gets the value of the pages property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPages() {
        return pages;
    }

    /**
     * Sets the value of the pages property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPages(String value) {
        this.pages = value;
    }

    /**
     * Gets the value of the displayNo property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getDisplayNo() {
        return displayNo;
    }

    /**
     * Sets the value of the displayNo property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setDisplayNo(String value) {
        this.displayNo = value;
    }

    /**
     * Gets the value of the articlePublicationDate property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getArticlePublicationDate() {
        return articlePublicationDate;
    }

    /**
     * Sets the value of the articlePublicationDate property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setArticlePublicationDate(String value) {
        this.articlePublicationDate = value;
    }

    /**
     * Gets the value of the seqNo property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSeqNo() {
        return seqNo;
    }

    /**
     * Sets the value of the seqNo property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSeqNo(String value) {
        this.seqNo = value;
    }

    /**
     * Gets the value of the title property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getTitle() {
        return title;
    }

    /**
     * Sets the value of the title property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setTitle(String value) {
        this.title = value;
    }

    /**
     * Gets the value of the subtitle property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSubtitle() {
        return subtitle;
    }

    /**
     * Sets the value of the subtitle property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSubtitle(String value) {
        this.subtitle = value;
    }

    /**
     * Gets the value of the pageFrom property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPageFrom() {
        return pageFrom;
    }

    /**
     * Sets the value of the pageFrom property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPageFrom(String value) {
        this.pageFrom = value;
    }

    /**
     * Gets the value of the pageTo property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPageTo() {
        return pageTo;
    }

    /**
     * Sets the value of the pageTo property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPageTo(String value) {
        this.pageTo = value;
    }

    /**
     * Gets the value of the doiNumber property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getDoiNumber() {
        return doiNumber;
    }

    /**
     * Sets the value of the doiNumber property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setDoiNumber(String value) {
        this.doiNumber = value;
    }

    /**
     * Gets the value of the url property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getUrl() {
        return url;
    }

    /**
     * Sets the value of the url property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setUrl(String value) {
        this.url = value;
    }

    /**
     * Gets the value of the foreignTitle property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getForeignTitle() {
        return foreignTitle;
    }

    /**
     * Sets the value of the foreignTitle property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setForeignTitle(String value) {
        this.foreignTitle = value;
    }

    /**
     * Gets the value of the foreignSubtitle property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getForeignSubtitle() {
        return foreignSubtitle;
    }

    /**
     * Sets the value of the foreignSubtitle property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setForeignSubtitle(String value) {
        this.foreignSubtitle = value;
    }

    /**
     * Gets the value of the language property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getLanguage() {
        return language;
    }

    /**
     * Sets the value of the language property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setLanguage(String value) {
        this.language = value;
    }

    /**
     * Gets the value of the abstract property.
     * 
     * @return
     *     possible object is
     *     {@link Abstract }
     *     
     */
    public Abstract getAbstract() {
        return _abstract;
    }

    /**
     * Sets the value of the abstract property.
     * 
     * @param value
     *     allowed object is
     *     {@link Abstract }
     *     
     */
    public void setAbstract(Abstract value) {
        this._abstract = value;
    }

    /**
     * Gets the value of the keywords property.
     * 
     * @return
     *     possible object is
     *     {@link Keywords }
     *     
     */
    public Keywords getKeywords() {
        return keywords;
    }

    /**
     * Sets the value of the keywords property.
     * 
     * @param value
     *     allowed object is
     *     {@link Keywords }
     *     
     */
    public void setKeywords(Keywords value) {
        this.keywords = value;
    }

    /**
     * Gets the value of the categories property.
     * 
     * @return
     *     possible object is
     *     {@link Categories }
     *     
     */
    public Categories getCategories() {
        return categories;
    }

    /**
     * Sets the value of the categories property.
     * 
     * @param value
     *     allowed object is
     *     {@link Categories }
     *     
     */
    public void setCategories(Categories value) {
        this.categories = value;
    }

    /**
     * Gets the value of the generalTerms property.
     * 
     * @return
     *     possible object is
     *     {@link GeneralTerms }
     *     
     */
    public GeneralTerms getGeneralTerms() {
        return generalTerms;
    }

    /**
     * Sets the value of the generalTerms property.
     * 
     * @param value
     *     allowed object is
     *     {@link GeneralTerms }
     *     
     */
    public void setGeneralTerms(GeneralTerms value) {
        this.generalTerms = value;
    }

    /**
     * Gets the value of the authors property.
     * 
     * @return
     *     possible object is
     *     {@link Authors }
     *     
     */
    public Authors getAuthors() {
        return authors;
    }

    /**
     * Sets the value of the authors property.
     * 
     * @param value
     *     allowed object is
     *     {@link Authors }
     *     
     */
    public void setAuthors(Authors value) {
        this.authors = value;
    }

    /**
     * Gets the value of the references property.
     * 
     * @return
     *     possible object is
     *     {@link References }
     *     
     */
    public References getReferences() {
        return references;
    }

    /**
     * Sets the value of the references property.
     * 
     * @param value
     *     allowed object is
     *     {@link References }
     *     
     */
    public void setReferences(References value) {
        this.references = value;
    }

    /**
     * Gets the value of the fulltext property.
     * 
     * @return
     *     possible object is
     *     {@link Fulltext }
     *     
     */
    public Fulltext getFulltext() {
        return fulltext;
    }

    /**
     * Sets the value of the fulltext property.
     * 
     * @param value
     *     allowed object is
     *     {@link Fulltext }
     *     
     */
    public void setFulltext(Fulltext value) {
        this.fulltext = value;
    }

    /**
     * Gets the value of the supplements property.
     * 
     * @return
     *     possible object is
     *     {@link Supplements }
     *     
     */
    public Supplements getSupplements() {
        return supplements;
    }

    /**
     * Sets the value of the supplements property.
     * 
     * @param value
     *     allowed object is
     *     {@link Supplements }
     *     
     */
    public void setSupplements(Supplements value) {
        this.supplements = value;
    }

    /**
     * Gets the value of the qualifiers property.
     * 
     * @return
     *     possible object is
     *     {@link Qualifiers }
     *     
     */
    public Qualifiers getQualifiers() {
        return qualifiers;
    }

    /**
     * Sets the value of the qualifiers property.
     * 
     * @param value
     *     allowed object is
     *     {@link Qualifiers }
     *     
     */
    public void setQualifiers(Qualifiers value) {
        this.qualifiers = value;
    }

    /**
     * Gets the value of the articleType property.
     * 
     * @return
     *     possible object is
     *     {@link ArticleType }
     *     
     */
    public ArticleType getArticleType() {
        return articleType;
    }

    /**
     * Sets the value of the articleType property.
     * 
     * @param value
     *     allowed object is
     *     {@link ArticleType }
     *     
     */
    public void setArticleType(ArticleType value) {
        this.articleType = value;
    }

    /**
     * Gets the value of the bestPaperText property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getBestPaperText() {
        return bestPaperText;
    }

    /**
     * Sets the value of the bestPaperText property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setBestPaperText(String value) {
        this.bestPaperText = value;
    }

    /**
     * Gets the value of the articleSponsors property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the articleSponsors property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getArticleSponsors().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link ArticleSponsors }
     * 
     * 
     */
    public List<ArticleSponsors> getArticleSponsors() {
        if (articleSponsors == null) {
            articleSponsors = new ArrayList<ArticleSponsors>();
        }
        return this.articleSponsors;
    }

    /**
     * Gets the value of the ccc property.
     * 
     * @return
     *     possible object is
     *     {@link Ccc }
     *     
     */
    public Ccc getCcc() {
        return ccc;
    }

    /**
     * Sets the value of the ccc property.
     * 
     * @param value
     *     allowed object is
     *     {@link Ccc }
     *     
     */
    public void setCcc(Ccc value) {
        this.ccc = value;
    }

    /**
     * Gets the value of the printOnDemand property.
     * 
     * @return
     *     possible object is
     *     {@link PrintOnDemand }
     *     
     */
    public PrintOnDemand getPrintOnDemand() {
        return printOnDemand;
    }

    /**
     * Sets the value of the printOnDemand property.
     * 
     * @param value
     *     allowed object is
     *     {@link PrintOnDemand }
     *     
     */
    public void setPrintOnDemand(PrintOnDemand value) {
        this.printOnDemand = value;
    }

}

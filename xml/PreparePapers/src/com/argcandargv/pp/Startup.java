package com.argcandargv.pp;

import java.io.IOException;
import java.util.ArrayList;
import java.util.regex.Pattern;

import javax.xml.bind.JAXBException;

import com.argcandargv.pp.entity.ArticleRec;
import com.argcandargv.pp.entity.Content;
import com.argcandargv.pp.entity.Fulltext;

/**
 * 本例打开存放XML的目录,解析其中所有XML(假定只有1级目录,要更多其实也不麻烦) 解析XML内容,放入entity中 使用时,直接调用内容即可
 * 
 * @author yu
 * 
 */

public class Startup {
	private static String dirInput = "/home/yu/Workspace/java/00/PreparePapers/data/input";
	private static String dirOutput = "/home/yu/Workspace/java/00/PreparePapers/data/output";

	/**
	 * input fileName, with the dir dirOutput. content is fileContent.
	 * 
	 * @param fileName
	 * @param Content
	 * @throws IOException
	 */
	public static void writeToFile(String fileName, String Content) throws IOException {
		java.io.File outputFile = new java.io.File(dirOutput + "/" + fileName);
		if (!outputFile.exists())
			outputFile.createNewFile();
		java.io.FileWriter fileWriter = new java.io.FileWriter(outputFile);
		fileWriter.write(Content);
		fileWriter.close();
	}

	public static void main(String args[]) throws InterruptedException {
		// String dirName = "/home/yu/Workspace/KEG/test/java/testdata/xmlsrc";
		String dirName = dirInput;
		long lasting = System.currentTimeMillis();
		int ifn = 0;
		long ipn = 0;
		java.io.File dir = new java.io.File(dirName);
		java.io.File[] fFileList = dir.listFiles();
		ArrayList<String> sFileList = new ArrayList<String>();
		Pattern isXmlFile = Pattern.compile("(.*?)\\.xml");
		for (java.io.File file : fFileList) {
			// if(file.getName().contains(".xml")){
			if (isXmlFile.matcher(file.getName()).find()) {
				sFileList.add((dirName.endsWith("/") ? dirName : dirName + "/")
						+ file.getName());
			}
		}
		int j = 0;
		for (String argv : args) { // options
			System.out.println((j++) + ":" + argv);
		}
		for (String file : sFileList) {
			ifn++;
			System.out.println("reading [" + file + "]...");
			XMLParse xp = new XMLParse();
			XMLData fd;
			try {
				fd = xp.parse(file);
			} catch (JAXBException e) {//
				e.printStackTrace();
				continue;
			}

			System.out.println("TS : " + fd.getProceeding().getTs() + " VER : "
					+ fd.getProceeding().getVer());
			// Pattern hasReferences =
			// Pattern.compile("(.*?)[r|R][eferences|EFERENCES](.*?)");

			for (Content ct : fd.getProceeding().getContent()) {
				for (ArticleRec atc : ct.getArticleRec()) {
					Fulltext ft = atc.getFulltext();
					System.out.println("body as follow:");

					System.out.println("[" + ft.getFtBody() + "]");

					String fileName = atc.getTitle();
					fileName.replace(' ', '_');
					fileName = new String(fileName+".txt");
					try {
						System.out.println("write into file ["+fileName+"]");
						writeToFile(fileName,ft.getFtBody());
					} catch (IOException e) {
						System.out.println("error:" + e);
					}
					System.out.println("done ... ");
					ipn++;
				}
			}
			System.out.println("[r.done]");
		}
		System.out.println("运行时间：" + (System.currentTimeMillis() - lasting)
				+ " 毫秒");
		System.out.println("文件数量" + ifn + "paper数量:" + ipn);

	}
}

import java.lang.*;
import java.io.*;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.util.*;

import org.apache.tika.exception.TikaException;
import org.apache.tika.metadata.Metadata;
import org.apache.tika.parser.ParseContext;
import org.apache.tika.parser.mp4.MP4Parser;
import org.apache.tika.sax.BodyContentHandler;

import org.xml.sax.SAXException;

import com.coremedia.iso.IsoTypeReader;
import com.googlecode.mp4parser.DataSource;

//javac -cp ".:tika-app-1.8.jar" Mp4Parse.java && java -cp ".:tika-app-1.8.jar" Mp4Parse ../file/teaser-vid.mp4

public class Mp4Parse {
	List<String> containers = Arrays.asList("moov", "trak", "mdia", "minf", "udta", "stbl");
	/** Parses the FileChannel, in the range [start, end) and prints the elements found
	 *
	 * Elements are printed, indented by "level" number of spaces.  If an element is
	 * a container, then its contents will be, recursively, printed, with a greater
	 * indentation.
	 *
	 * @param fc
	 * @param level
	 * @param start
	 * @param end
	 * @throws IOException
	 */
	public void print(FileChannel fc, int level, long start, long end)
	throws IOException {
		fc.position(start);
		if(end <= 0) {
			end = start + fc.size();
			System.out.println("Set END to: " + end);
		}
		while (end - fc.position() > 8) {
			long begin = fc.position();
			ByteBuffer bb = ByteBuffer.allocate(8);
			fc.read(bb);
			bb.rewind();
			long size = IsoTypeReader.readUInt32(bb);
			String type = IsoTypeReader.read4cc(bb);
			long fin = begin + size;
			for (int i = 0; i < level; i++) {
				System.out.print(" ");
			}
			System.out.println(type + "@" + (begin) + " sz: " + size);
			if (containers.contains(type)) {
				print(fc, level + 1, begin + 8, fin);
				if(fc.position() != fin) {
					System.out.println("End of container contents @ " + fc.position());
					System.out.println("FIN = " + fin);
				}
			}

			fc.position(fin);
		}
	}
	public static void main(final String[] args)
	throws IOException, SAXException, TikaException {
		BodyContentHandler handler = new BodyContentHandler();
		Metadata metadata = new Metadata();
		FileInputStream fis = new FileInputStream(new File(args[0]));
		ParseContext pcontext = new ParseContext();
		MP4Parser MP4Parser = new MP4Parser();
		MP4Parser.parse(fis, handler, metadata, pcontext);

		System.out.println("Handler: "+handler.toString());
		System.out.println("--- Metadata ---");
		String[] metadataNames = metadata.names();
		for (String name : metadataNames) {
			System.out.println(name+": "+metadata.get(name));
		}
		System.out.println();
		System.out.println("--- Content ---");
        System.out.println("input sz: "+fis.getChannel().size());
        new Mp4Parse().print(fis.getChannel(), 0, 0, 0);
	}
}

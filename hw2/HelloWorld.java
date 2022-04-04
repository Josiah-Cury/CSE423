// Comment test

/*
	Multiline
	comment
	test
*/

public class HelloWorld {
	public static void main(String[] args) {
		float t = 12.34;
		int i = 2;
		char d = 'd';
		char c = '\n';
		System.out.println("Hello, World");
		switch (c) {
			case 1: case 2:
			   System.out.println("1\tor\t2\n"); break;
			default:
			   System.out.println("\'Some \\more \"text\n");
		}
	}
}


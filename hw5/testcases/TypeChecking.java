public class TypeChecking {
    //this is a comment

    /* Multiline comment
    goes this long.*/

    char l;
	int k = 3;

	public static int foo(int x, String s, boolean b) {
		int y = 5;
		boolean t;
		boolean f;
		x = y * -x;
		x = x + -y * x;
		b = x < y;
		b = t && f;
		//x = y * x;
		//x = -x;
		//b = !b;

		System.out.print(s);

		return x;
	}

    public static void main(String argv[]) {
      System.out.println("hello, jzero!");
	  InputStream.read();
      int f = 1;
	  String str = "Hello this is a string";
	  f++;

	  f = foo(f, str);
   }
}

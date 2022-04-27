public class Hello {
    //this is a comment

    /* Multiline comment
    goes this long.*/
	// public static int foo(int i) {
	// public static int foo(int i, int k) {
	public static int foo(int i, int k, int l) {
		// return i;
		// return i + k;
		return i + k - l;
	}

	//int k = 1;
    public static void main(String argv[]) {
      int i = 5;

	  String s = "Hello's";

	  i = i * i + 1;
	  i = i + (i - 2);
	  i = i / i + 23;

	  // foo(i);
	  // foo(i, 5);
	  foo(i, 5, 3);

	  System.out.println(s);
	  System.out.println("test1");

	  // System.out.println("test2");
	  // System.out.println("test3");

   }
}

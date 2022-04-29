public class Hello {
    //this is a comment

    /* Multiline comment
    goes this long.*/
	// public static int foo(int i) {
	// public static int foo(int i, int k) {
	public static int foo(int i, int k, int l) {
		// return i;
		// return i + k;
		i = k * l + (l - k);
		return i + k - l;
	}

	public static String greet(String s, String t, String st) {
		return s + t + st;
	}

	//int k = 1;
    public static void main(String argv[]) {
      int i = 5;

	  String s = "Hello " + "World\n";

	  if (i < 10) {
		  i = i * i + 1;
	  }

	  i = i + (i - 2);
	  i = i / i + 23;

	  // foo(i);
	  // foo(i, 5);
	  foo(i, 5+3, 3);

	  // greet("s", " string", 12 + "\tend");

	  // System.out.println(s);
	  System.out.println("Does this count?");
	  System.out.println("test1" + 23);

	  // System.out.println("test2");
	  // System.out.println("test3");

   }
}

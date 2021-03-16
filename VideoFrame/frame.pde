public class PFrame extends JFrame {
  //JFrame frame;

  secondApplet s;
  public PFrame() {
    setBounds(0, 0, 940, 370);
    s = new secondApplet();
    add(s);
    s.init();
    show();
    //  frame.setVisible(true);
  }
}

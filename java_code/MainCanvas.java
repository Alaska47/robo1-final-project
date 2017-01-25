import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.util.*;

public class MainCanvas {
    
   private static ArrayList<double[]> pointsToDraw = new ArrayList<double[]>();
    
   private static double[] startingPoint;
   private static boolean liveDraw = false;
   private static boolean reDraw = false;
    
   public static void main(String args[]) throws Exception {
      JFrame f = new JFrame("");
      f.setSize(300, 300);
      f.setLocation(300, 300);
      f.setResizable(false);
      JPanel p = 
         new JPanel() {
            
            {
               addMouseListener(
                  new MouseAdapter() {
                     public void mousePressed(MouseEvent e) {
                        if(startingPoint == null)
                           liveDraw = true;
                        if(liveDraw) {
                           Point pointer = e.getPoint();
                           startingPoint = new double[2];
                           startingPoint[0] = pointer.getX();
                           startingPoint[1] = pointer.getY();
                           pointsToDraw.add(startingPoint);
                        }
                     }
                  
                     public void mouseReleased(MouseEvent e) {
                        liveDraw = false;
                        reDraw = false;
                        //repaint();
                     }
                  });
               addMouseMotionListener(
                  new MouseMotionAdapter() {
                     public void mouseMoved(MouseEvent e) {
                        //pointEnd = e.getPoint();
                     }
                  
                     public void mouseDragged(MouseEvent e) {
                        double[] currentPoint = {e.getX(), e.getY()};
                        double[] compare = pointsToDraw.get(pointsToDraw.size() - 1);
                        if (distanceBetweenPoints(compare[0], currentPoint[0], compare[1], currentPoint[1]) > 35) {
                           if (distanceBetweenPoints(startingPoint[0], currentPoint[0], startingPoint[1], currentPoint[1]) < 25) {
                              //path.lineTo(startingPoint[0], startingPoint[1]);
                              pointsToDraw.add(startingPoint);
                           } 
                           else {
                              //path.lineTo(currentPoint[0], currentPoint[1]);
                              pointsToDraw.add(currentPoint);
                           //this.setEnabled(false);
                           }
                        
                        }
                        repaint();
                     }
                  });
            }
            public void paint(Graphics g) {
               if(reDraw) {
                  super.paint(g);
               }
               if (liveDraw) {
                  super.paint(g);
                  g.setColor(Color.BLACK);
                  if(pointsToDraw.size() > 2) {
                     ArrayList<double[]> pointsToDraw1 = pointsToDraw;
                     for(int x = pointsToDraw.size(); x > 1; x--) {
                        double[] prev = pointsToDraw.get(pointsToDraw.size() - x);
                        double[] now = pointsToDraw.get(pointsToDraw.size() - x + 1);
                        g.drawLine((int) prev[0],(int) prev[1],(int) now[0],(int) now[1]);
                     }
                  }
               }
            }
         };
      f.add(p, BorderLayout.CENTER);
      JPanel optionsPanel = new JPanel();
      
      JButton reset = new JButton("Reset");
      reset.addActionListener(
         new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               reDraw = true;
               startingPoint = null;
               pointsToDraw = new ArrayList<double[]>();
               f.validate();
               
            }
         });
         
      optionsPanel.add(reset);
      JButton send = new JButton("Send");
      send.addActionListener(
         new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               String a = getEncodedData();
               System.out.println(preProcessData(a));
            }
         });
      optionsPanel.add(send);
      
      f.add(optionsPanel, BorderLayout.SOUTH);
      
      f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      f.setVisible(true); 
   }
    
   private static double distanceBetweenPoints(double x1, double x2, double y1, double y2) {
      return (Math.sqrt(((Math.pow((x2 -x1), 2.0)) + (Math.pow((y2 - y1), 2.0)))));
   }
   
   private static String getEncodedData() {
      ArrayList<Vector> g = new ArrayList<>();
      g.add(new Vector(-1, 0));
      for(int i = 0; i < pointsToDraw.size() - 1; i += 1) {
         double[] fir = pointsToDraw.get(i);
         double[] sec = pointsToDraw.get(i + 1);
         g.add(new Vector((sec[0] - fir[0]), (fir[1] - sec[1])));
      }
      DataEncoder d = new DataEncoder(g);
      d.processData();
      return d.returnData();
   }
   
   private static String preProcessData(String data) {
      String finalData = "";
      String[] arr = data.split("\n");
      for(String a : arr) {
         String[] sub = a.split(": ");
         double d = Double.parseDouble(sub[1]);
         if(sub[0].equals("ANGLE")) {
            finalData += (int) d + ",";
         } 
         else {
            finalData += round(d / 35.0, 2) + ",";
         }
      }
      return finalData.substring(0, finalData.length() - 1);
   }
    
   public static double round(double value, int places) {
      if (places < 0) throw new IllegalArgumentException();
   
      long factor = (long) Math.pow(10, places);
      value = value * factor;
      long tmp = Math.round(value);
      return (double) tmp / factor;
   }
}
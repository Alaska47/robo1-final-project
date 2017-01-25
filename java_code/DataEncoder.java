
import java.util.ArrayList;

/**
 * Created by anees on 1/17/2017.
 */
public class DataEncoder {

   private ArrayList<String> commands = new ArrayList<>();
   private ArrayList<Vector> inp;

   public DataEncoder(ArrayList<Vector> a) {
      inp = a;
   }

   public void processData() {
      for(int i = 0; i < inp.size() - 1; i++) {
         Vector curr = inp.get(i);
         if(i != 0) {
            commands.add("MAG: " + curr.magnitude());
         }
         double angle = 0.0;
         if(i == 0) {
            Vector g = inp.get(i + 1);
            if(!(g.getX() < 0 && g.getY() < 0)) {
               angle = 360.0 - curr.angle(inp.get(i + 1));
            } 
            else {
               angle = curr.angle(inp.get(i + 1));
            }
            commands.add("ANGLE: " + angle);
         } 
         else {
            angle = curr.angle(inp.get(i + 1));
            angle = 180.0 - angle;
            if(Math.abs(180.0 - angle) > 10) {
               commands.add("ANGLE: " + angle);
            }
         }
         
      }
      commands.add("MAG: " + inp.get(inp.size() - 1).magnitude());
   
   }

   public String returnData() {
      String out = "";
      for(String d : commands) {
         out += d.toString() + "\n";
      }
      return out.trim();
   }

}

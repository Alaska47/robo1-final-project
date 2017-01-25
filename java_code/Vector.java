
/**
 * Created by anees on 1/17/2017.
 */
public class Vector {
    private double x_comp;
    private double y_comp;

    public Vector(double x, double y) {
        x_comp = x;
        y_comp = y;
    }

    public double magnitude() {
        return Math.sqrt((Math.pow(x_comp, 2.0) + Math.pow(y_comp, 2.0)));
    }

    public double angle(Vector b) {
        Vector neg = new Vector(-x_comp,-y_comp);
        double dotProduct = neg.getX() * b.getX() + neg.getY() * b.getY();
        double magA = neg.magnitude();
        double magB = b.magnitude();
        return Math.acos(dotProduct / (magA * magB))  * (180/Math.PI);
    }

    public double getX() {
        return x_comp;
    }

    public double getY() {
        return y_comp;
    }

    public String toString() {
        return "<" + x_comp + "," + y_comp + ">";
    }

}

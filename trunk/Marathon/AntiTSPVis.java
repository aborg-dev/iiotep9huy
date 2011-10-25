import java.util.*;
import java.awt.*;
import java.awt.image.*;
import java.io.*;
import java.security.*;
import javax.swing.*;
import javax.imageio.*;

public class AntiTSPVis {
    static int n_points;	//number of points in tour
    static int x_points ;       // extra points
    static int t_points ;       // total points
    
    static int startx , starty ; // start point
    static int[] fx ;
    static int[] fy ; // all fixed points, including the start
     
    int[] ret;       // point coordinates returned
    static int [] tour ; // sorted return 
    static int [] tour_i ;
    static String points_arg ; 
    static String start_x_arg ; 
    static String start_y_arg ; 
    static boolean city_dot ;
    static boolean drawScore ;
    static boolean tour_label ;
    static boolean return_label ;
    static String fixed_arg ;
    SecureRandom r;
    // -----------------------------------------
    static int join(int r, int g, int b){
        return (r<<16)|(g<<8)|b;
    }
    // -----------------------------------------
    void generate(String seed) {
      try {
        r = SecureRandom.getInstance("SHA1PRNG");
        r.setSeed(Long.parseLong(seed));
        n_points = (int) ( 0.5 + Math.pow ( 10.0 , 1+r.nextDouble()*3 ) ) ;

        if ( seed.equals("1") )
           {
           n_points = 10 ;
           }
        if ( seed.equals("2") )
           {
           n_points = 100 ;
           }
        if ( seed.equals("3") )
           {
           n_points = 1000 ;
           }
        if ( seed.equals("4") )
           {
           n_points = 10000 ;
           }
        
        if ( points_arg != null )
           n_points = Integer.parseInt(points_arg) ;

        int nfp = 3+r.nextInt ( Math.min(10,n_points/3)-2 ) ;
        if ( fixed_arg != null )
           nfp = 1 + Math.min ( 10 , 
                                Math.max ( 0 , 
                                           Integer.parseInt(fixed_arg) ) ) ;
        fx = new int [nfp] ;
        fy = new int [nfp] ;
        x_points = nfp -1 ;
        t_points = n_points+x_points ;
        
        for ( int p = 0 ; p < nfp ; p ++ )
           {
           fx[p] = r.nextInt ( 1000000001 ) ;
           fy[p] = r.nextInt ( 1000000001 ) ;
           }

        if ( start_x_arg != null )
           fx[0] = Integer.parseInt(start_x_arg) ;
        if ( start_y_arg != null )
           fy[0] = Integer.parseInt(start_y_arg) ;

        startx = fx[0] ;
        starty = fy[0] ;
      }
      catch (Exception e) { 
        System.err.println("An exception occured while generating the test case.");
        e.printStackTrace(); 
      }
    }
    // -----------------------------------------
    public double runTest(String seed) {
      try {
        generate(seed);

        //pass the params and get the result
        ret = placeLocations(n_points,fx,fy);
        if ( ret.length != n_points*2 )
           {
           System.err.println ( "return must contain " + n_points*2 + 
                                " elements." ) ;
           return 0.0 ;
           }
        for ( int i = 0 ; i < ret.length ; i ++ )
           if ( ret[i] < 0 || ret[i] > 1000000000 )
              {
              System.err.println ( "ret element " + i + 
                " must be between 0 and 1,000,000,000, inclusive." ) ;
              return 0.0 ;
              }
        
        tour = new int [ret.length+fx.length+fy.length-2] ;
        tour_i = new int [ret.length/2+fx.length] ;
        double pathlength = 0 ;
        long cx = startx, cy = starty;
        int ni = -1 ;
        long d2 = 10 ;
        int[] pt = new int [t_points*2] ;
        boolean [] used = new boolean [t_points] ;

        for ( int i = 0 ; i < ret.length ; i ++ )
           pt[i] = ret[i] ;
        for ( int i = 1 ; i < fx.length ; i ++ )
           {
           pt[i+i+ret.length-2] = fx[i] ;
           pt[i+i+ret.length-1] = fy[i] ;
           }
        
        for ( int k = 0 ; k < t_points ; k ++ )
           {
           d2 = Long.MAX_VALUE ;
           for ( int i = 0 ; i < t_points ; i ++ )
              if ( !used[i] && (pt[i+i]-cx)*(pt[i+i]-cx) +
                   (pt[i+i+1]-cy)*(pt[i+i+1]-cy) < d2 )
                 {
                 d2 = (pt[i+i]-cx)*(pt[i+i]-cx) +
                    (pt[i+i+1]-cy)*(pt[i+i+1]-cy) ;
                 ni = i ;
                 }
           pathlength += Math.sqrt((double)d2) * 1e-9 ;
           
           used[ni] = true ;
           cx = pt[ni+ni] ;
           cy = pt[ni+ni+1] ;
           tour_i[k] = ni ;
           tour[k+k] = (int)cx ;
           tour[k+k+1] = -(int)cy ;;
           }
        long d = (startx-cx)*(startx-cx) +
           (starty-cy)*(starty-cy)  ;
        pathlength +=  Math.sqrt((double)d) * 1e-9;
        
        //estimate the result
        return pathlength  ;
      }
      catch (Exception e) { 
        System.err.println("An exception occurred while trying to get your program's results.");
        e.printStackTrace(); 
        return 0.0;
      }
    }
// ------------- visualization part ------------
    static String exec;
    static String file;
    static int disp_size ;
    static int disp_off_x , disp_off_y ;
    static String diff;
    static Process proc;
    InputStream is;
    OutputStream os;
    // -----------------------------------------
    public int[] placeLocations(int n, int[] fx, int[] fy) throws IOException {
        int i;
        //imitate passing params to getMap
        StringBuffer sb = new StringBuffer();
        sb.append(n).append('\n');
        sb.append(fx.length).append('\n');
        for ( i = 0 ; i < fx.length ; i ++ )
           sb.append(fx[i]).append('\n');
        sb.append(fy.length).append('\n');
        for ( i = 0 ; i < fy.length ; i ++ )
           sb.append(fy[i]).append('\n');
        os.write(sb.toString().getBytes());
        os.flush();
        //imitate solution's calls of measure
        BufferedReader br = new BufferedReader(new InputStreamReader(is));
        //imitate receiving return from placeLocations
        //n_points 
        int count = Integer.parseInt(br.readLine()) ;
        if ( count != n_points * 2 )
           {
           System.out.println ( "Array of " + n_points*2 + " elements expected" ) ;
           System.out.println ( "Score = 0" ) ;
           System.exit ( 666 ) ;
           }
        int[] ret = new int [n_points*2];
        for (i=0; i<n_points*2; i++)
            ret[i] = Integer.parseInt(br.readLine());
        return ret;
    }
    // -----------------------------------------
// ****************************************************************************
    BufferedImage drawTour ( double score , String seed ) 
// ****************************************************************************
 {
//draws a map in colors 0..cmax
int title_space = 0 ;
if ( drawScore ) title_space = 10 ;

BufferedImage bi ;
bi = new BufferedImage ( disp_size+35,
                         disp_size+20+title_space,
                         BufferedImage.TYPE_INT_RGB);
Graphics2D g = (Graphics2D)bi.getGraphics() ;

int xoff = 10 ;
int yoff = disp_size - xoff ;
int rez = disp_size - xoff * 2 ;
double scale = 1.0e-9 * rez ;

g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, 
                   RenderingHints.VALUE_ANTIALIAS_ON);    

g.setColor(Color.YELLOW) ;
g.fillRect(0,0,bi.getWidth(),bi.getHeight()) ;
g.setColor(Color.WHITE) ;
g.fillRect(xoff,yoff-rez,rez,rez) ;

g.setColor(new Color ( 0x7f7f7f ) ) ;
g.drawLine ( (int)(startx*scale+xoff ) ,
             (int)(starty*-scale+yoff ) ,
             (int)(tour[0]*scale+xoff ) ,
             (int)(tour[1]*scale+yoff ) ) ;
Color c = null ;
for ( int i = 0 ; i < t_points*2-2 ; i += 2 )
   {
   c = new Color ( 0x10101 * ( 127 - i*64/t_points ) ) ;
   g.setColor ( c ) ;
   g.drawLine ( (int)(tour[i]*scale+xoff ) ,
                (int)(tour[i+1]*scale+yoff ) ,
                (int)(tour[i+2]*scale+xoff ) ,
                (int)(tour[i+3]*scale+yoff ) ) ;
   if ( tour_label ) 
      g.drawString(""+(i/2+1) , 
                   (int)(tour[i]*scale+xoff ) ,
                   (int)(tour[i+1]*scale+yoff ) ) ;
   if ( return_label ) 
      g.drawString( (tour_i[i/2] < n_points) ?
                    (""+(tour_i[i/2]) ) : ("-"+(tour_i[i/2]-n_points+1) ) , 
                   (int)(tour[i]*scale+xoff ) ,
                   (int)(tour[i+1]*scale+yoff ) ) ;
   if ( tour_i[i/2] >= n_points )
      {
      g.setColor(Color.RED) ;
      g.fillOval((int)(tour[i]*scale+xoff-3 ) ,
                 (int)(tour[i+1]*scale+yoff-3 ) , 7 , 7 ) ;
      g.setColor(c) ;
      }
   else
      {
      if ( city_dot )
         {
         g.fillOval((int)(tour[i]*scale+xoff-2 ) ,
                    (int)(tour[i+1]*scale+yoff-2 ) , 5 , 5 ) ;
         }
      }
   }

if ( tour_label ) 
   g.drawString ( "" + t_points , 
                  (int)(tour[t_points*2-2]*scale+xoff ) ,
                  (int)(tour[t_points*2-1]*scale+yoff ) ) ;
if ( return_label ) 
   g.drawString( (tour_i[t_points-1] < n_points ?
                  (""+(tour_i[t_points-1]) ) : 
                  ("-"+(tour_i[t_points-1]-n_points+1) ) ) , 
                (int)(tour[tour.length-2]*scale+xoff ) ,
                (int)(tour[tour.length-1]*scale+yoff ) ) ;
if ( tour_i[t_points-1] >= n_points )
   {
   g.setColor(Color.RED) ;
   g.fillOval((int)(tour[t_points*2-2]*scale+xoff-3 ) ,
              (int)(tour[t_points*2-1]*scale+yoff-3 ) , 7 , 7 ) ;
   g.setColor(c) ;
   }
else
   {
   if ( city_dot )
      {
      g.fillOval((int)(tour[t_points*2-2]*scale+xoff-2 ) ,
                 (int)(tour[t_points*2-1]*scale+yoff-2 ) , 5 , 5 ) ;
      }
   }

g.drawLine ( (int)(startx*scale+xoff ) ,
             (int)(starty*-scale+yoff ) ,
             (int)(tour[tour.length-2]*scale+xoff ) ,
             (int)(tour[tour.length-1]*scale+yoff ) ) ;

g.setColor(Color.RED) ;
g.fillOval((int)(startx*scale+xoff-5 ) ,
           (int)(starty*-scale+yoff-5 ) , 11 , 11 ) ;

if ( drawScore ) 
   {
   g.setColor ( Color.BLUE ) ;
   g.drawString ( "Score " + score + " seed = " + seed ,
                  14 , disp_size + 8 ) ;
   }
return bi;
 }
// ****************************************************************************
    // -----------------------------------------
    public AntiTSPVis ( String seed) throws IOException {
// ****************************************************************************
        //interface for runTest
        if (exec != null) {
            try {
                Runtime rt = Runtime.getRuntime();
                proc = rt.exec(exec);
                os = proc.getOutputStream();
                is = proc.getInputStream();
                new ErrorReader(proc.getErrorStream()).start();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        double sc = runTest(seed) ;
        System.out.println("N = " + n_points ) ;
        System.out.println ( "Initial ("+fx[0]+". "+fy[0]+")" ) ;
        for ( int i = 1 ; i <= x_points ; i ++ )
           System.out.println ( "Fixed ("+fx[i]+". "+fy[i]+")" ) ;
        System.out.println ( "Score = "+sc);

        int shift = 0;
        //visualize, if required
        if (file != null)
        {   //draw the tour
        final BufferedImage bi = drawTour( sc , seed );
            if (file.equals("-")) 
            {   JFrame jf = new JFrame();
                jf.setLocation(disp_off_x,disp_off_y+shift);
                jf.setSize(disp_size+35,disp_size+10+(drawScore?40:20));
                shift += disp_size+30;
                jf.getContentPane().add(new JPanel(){
                    public void paint(Graphics g){
                        g.drawImage(bi,0,0,null);
                    }
                });
                jf.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                jf.setVisible(true);
            }
            else
                ImageIO.write(bi,"png",new File(file+".png"));
        }
    }
    // -----------------------------------------
    public static void main(String[] args) throws IOException {
        String seed = "1";
        String display_size = "1000" ;
        for (int i = 0; i<args.length; i++)
        {   if (args[i].equals("-seed") || args[i].equals("-s"))
                seed = args[++i];
            if (args[i].equals("-exec") || args[i].equals("-x"))
                exec = args[++i];
            if (args[i].equals("-vis") || args[i].equals("-v"))
                file = args[++i];
            if (args[i].equals("-vis_size") || args[i].equals("-sz") ||
                args[i].equals("-vs"))
               display_size = args[++i];
            if (args[i].equals("-points") || args[i].equals("-n"))
               points_arg = args[++i];
            if (args[i].equals("-initial") || args[i].equals("-xy"))
               {
               start_x_arg = args[++i];
               start_y_arg = args[++i];
               }
            if (args[i].equals("-offset") || args[i].equals("-o"))
               {
               disp_off_x = Integer.parseInt(args[++i]);
               disp_off_y = Integer.parseInt(args[++i]);
               }
            if (args[i].equals("-tour_label") || args[i].equals("-tl"))
               tour_label = true;
            if (args[i].equals("-dot") || args[i].equals("-d"))
               city_dot = true;
            if (args[i].equals("-return_label") || args[i].equals("-rl"))
               return_label = true;
            if (args[i].equals("-score") || args[i].equals("-sc"))
               drawScore = true;
            if (args[i].equals("-fixed") || args[i].equals("-f"))
               fixed_arg = args[++i] ;
        }
        disp_size = Integer.parseInt(display_size) ;
        AntiTSPVis f = new AntiTSPVis(seed);
    }
    // -----------------------------------------
    void addFatalError(String message) {
        System.out.println(message);
    }
}

class ErrorReader extends Thread{
    InputStream error;
    public ErrorReader(InputStream is) {
        error = is;
    }
    public void run() {
        try {
            byte[] ch = new byte[50000];
            int read;
            while ((read = error.read(ch)) > 0)
            {   String s = new String(ch,0,read);
                System.out.print(s);
                System.out.flush();
            }
        } catch(Exception e) { }
    }
}

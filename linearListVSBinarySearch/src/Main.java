/*Linear List VS BST*/
/*23101150 Kang Min Chan*/
// 이 프로그램은 LinkedList와 TreeSet의 삽입 성능을 비교해보는 JAVA SWING 프로그램입니다.
import javax.swing.*;
import java.awt.*;
import java.util.*;
import java.util.List;

public class Main extends JPanel {
    // 배열에 테스트할 데이터 크기들을 정의
    private final int[] sizes = {1000, 5000, 10000, 20000, 40000};
    // 각 크기에 대한 LinkedList 삽입 시간(ms)을 저장할 배열
    private final double[] linearTimes = new double[sizes.length];
    // 각 크기에 대한 TreeSet 삽입 시간(ms)을 저장할 배열
    private final double[] bstTimes = new double[sizes.length];

    // Constructor -> 테스트를 실행하고 패널의 기본 크기를 설정
    public Main() {
        runTests();  // 성능 테스트 수행
        setPreferredSize(new Dimension(800, 600));  // 패널 크기 설정
    }

    // 성능 테스트 메서드
    public void runTests() {
        Random rand = new Random();  // 랜덤 값 생성을 위한 객체
        for (int i = 0; i < sizes.length; i++) {
            int n = sizes[i];  // 현재 테스트할 데이터 크기
            List<Integer> linearList = new LinkedList<>();  // 선형 리스트 생성

            // LinkedList에 정렬된 위치에 요소 삽입하며 시간 측정
            long start = System.nanoTime();  // 시작 시간 (나노초)
            for (int j = 0; j < n; j++) {
                int val = rand.nextInt();  // 랜덤 값 생성
                int idx = 0;  // 삽입할 인덱스 초기화
                // 리스트를 돌면서 val이 들어갈 위치 찾기
                for (int num : linearList) {
                    if (val < num) break;  // 현재 값보다 작으면 멈춤
                    idx++;
                }
                linearList.add(idx, val);  // 해당 위치에 값 삽입
            }
            long end = System.nanoTime();  // 종료 시간
            // 밀리초 단위로 변환하여 저장
            linearTimes[i] = (end - start) / 1_000_000.0;

            // TreeSet 생성 및 삽입 시간 측정
            TreeSet<Integer> bst = new TreeSet<>();
            start = System.nanoTime();
            for (int j = 0; j < n; j++) {
                bst.add(rand.nextInt());  // 랜덤 값 삽입
            }
            end = System.nanoTime();
            // 밀리초 단위로 변환하여 저장
            bstTimes[i] = (end - start) / 1_000_000.0;

            // 콘솔에 결과 출력
            System.out.printf("[%d] Linear: %.2f ms, BST: %.2f ms\n", n, linearTimes[i], bstTimes[i]);
        }
    }

    // 그래프를 그리는 메서드: JPanel의 paintComponent를 Override했음
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);  // 배경 지우기 등 기본 처리
        Graphics2D g2 = (Graphics2D) g;  // Graphics2D로 형변환하여 고급 그래픽 사용

        int width = getWidth();   // 패널 가로 크기
        int height = getHeight(); // 패널 세로 크기
        // 각 막대의 너비 계산 (총 항목 수 * 3 공간 고려)
        int barWidth = width / (sizes.length * 3);
        // 시간 데이터의 최댓값을 찾아 비율 계산에 사용
        double max = Arrays.stream(linearTimes).max().orElse(1);
        max = Math.max(max, Arrays.stream(bstTimes).max().orElse(1));

        // 그래프 제목 및 범례
        g2.setFont(new Font("SansSerif", Font.PLAIN, 12));
        g2.setColor(Color.BLACK);
        g2.drawString("Blue: Linear List   Green: BST", 20, 20);

        // 각 크기에 대해 막대 그래프 그리기
        for (int i = 0; i < sizes.length; i++) {
            // 막대 시작 x 좌표
            int x1 = i * barWidth * 3 + barWidth;
            // 막대 높이를 패널 높이에 비례하여 계산
            int h1 = (int) ((linearTimes[i] / max) * (height - 80));
            int h2 = (int) ((bstTimes[i] / max) * (height - 80));
            // 너무 작아 보이지 않도록 최소 높이 설정
            h1 = Math.max(h1, 4);
            h2 = Math.max(h2, 4);

            // 선형 리스트 막대 (파란색)
            g2.setColor(Color.BLUE);
            g2.fillRect(x1, height - h1 - 30, barWidth, h1);
            g2.setColor(Color.BLACK);
            // 막대 위에 시간 라벨 출력
            g2.drawString(String.format("%.2f ms", linearTimes[i]), x1, height - h1 - 35);

            // BST 막대 (녹색)
            g2.setColor(Color.GREEN);
            g2.fillRect(x1 + barWidth, height - h2 - 30, barWidth, h2);
            g2.setColor(Color.BLACK);
            // 막대 위에 시간 라벨 출력
            g2.drawString(String.format("%.2f ms", bstTimes[i]), x1 + barWidth, height - h2 - 35);
        }
    }

    // JFrame 생성 및 Main 패널 추가
    public static void main(String[] args) {
        JFrame frame = new JFrame("Speed Comparison Graph");  // 윈도우 타이틀 설정
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  // 닫기 버튼 클릭 시 종료
        frame.getContentPane().add(new Main());  // Main 패널 추가
        frame.pack();  // 컴포넌트에 맞춰 윈도우 크기 조정
        frame.setLocationRelativeTo(null);  // 화면 가운데에 배치
        frame.setVisible(true);  // 윈도우 표시
    }
}

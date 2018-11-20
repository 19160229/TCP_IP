import java.util.ArrayList;
public class TreeNode {
    private String id;
    private String pid;
    private String name;
    private String remark;
    private ArrayList<TreeNode> children = new ArrayList<TreeNode>();
    public TreeNode(String id, String pid, String name, String remark) {
        this.id = id;
        this.pid = pid;
        this.name = name;
        this.remark = remark;
    }
    public void add(TreeNode node) {//递归添加节点
        if ("0".equals(node.pid)) {
            this.children.add(node);
        } else if (node.pid.equals(this.id)){
            this.children.add(node);
        } else {
            for (TreeNode tmp_node : children) {
                tmp_node.add(node);
            }
        }
    }
    public String getId() {
        return id;
    }
    public void setId(String id) {
        this.id = id;
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }

    public String getRemark() {
        return remark;
    }
    public void setRemark(String remark) {
        this.remark = remark;
    }
    public ArrayList<TreeNode> getChildren() {
        return children;
    }
    public void setChildren(ArrayList<TreeNode> children) {
        this.children = children;
    }
}
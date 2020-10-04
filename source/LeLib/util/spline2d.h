#ifndef SPLINES_HPP
#define SPLINES_HPP

#include <vector>
#include <QVector3D>

enum Node_e {
    eUNIFORM,
    eOPEN_UNIFORM ///< Connected to the first and last control points
};

// END ESPline ================================================================

/**
 * @class Spline
 *
 * @brief Handling spline curves of arbitrary dimensions
 * @note This class use the efficient blossom algorithm to compute a position on
 * the curve.
 *
 * @tparam QVector3D : type of a point operators such as '+' '*' must be correctly
 * overloaded. The default constructor must be defined to return the
 * null vector (0, 0 ,0 ...)
 * @tparam float ; floating point reprensentation of the points
 * (float, double etc.)
 */

class Spline2D {
public:

    /// Type of the nodal vector
    /// @param k : order of the spline (minimum is two)
    /// @param node_type : nodal vector type (uniform, open_uniform)
    /// This will define the behavior of the spline with its control points
    /// as well as its speed according to its parameter.
    Spline2D(int k = 2, Node_e node_type = eOPEN_UNIFORM);

    /// Set the position of the spline control points.
    void set_ctrl_points(const std::vector<QVector3D>& point);

    /// Get the control points of the spline
    void get_ctrl_points(std::vector<QVector3D>& points) const;

    /// The the nodal vector type
    void set_node_type( Node_e type);

    /// Evaluate position of the spline
    /// @param u : curve parameter ranging from [0; 1]
    QVector3D eval_f(float u) const;

    /// Evaluate speed of the spline
    QVector3D eval_df(float u) const;

    int get_order() const { return _k; }

private:
    // -------------------------------------------------------------------------
    /// @name Class tools
    // -------------------------------------------------------------------------

    void assert_splines() const;

    /// set value and size of the nodal vector depending on the current number
    /// of control points
    void set_nodal_vector();

    /// Set values of the nodal vector to be uniform
    void set_node_to_uniform();

    /// Set values of the nodal vector to be open uniform
    void set_node_to_open_uniform();

    /// Evaluate the equation of a splines using the blossom algorithm
    /// @param u : the curve parameter which range from the values
    /// [node[k-1]; node[point.size()]]
    /// @param point : the control points which size must be at least equal to
    /// the order of the spline (point.size() >= k)
    /// @param k : the spline order (degree == k-1)
    /// @param node : the nodal vector which defines the speed of the spline
    /// parameter u. The nodal vector size must be equal to (k + point.size())
    /// @param off : offset to apply to the nodal vector 'node' before reading
    /// from it. this is useful to compute derivatives.
    QVector3D eval(float u,
                 const std::vector<QVector3D>& point,
                 int k,
                 const std::vector<float>& node,
                 int off = 0) const;

    QVector3D eval_rec(float u,
                     std::vector<QVector3D> p_in,
                     int k,
                     std::vector<float> node_in) const;

    // -------------------------------------------------------------------------
    /// @name attributes
    // -------------------------------------------------------------------------

    Node_e _node_type;    ///< Nodal vector type
    int _k;                        ///< spline order
    std::vector<QVector3D> _point;   ///< Control points
    std::vector<QVector3D> _vec;     ///< Control points differences
    std::vector<float>  _node;    ///< Nodal vector
};


#endif // SPLINES_HPP

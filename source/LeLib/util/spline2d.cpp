#include "spline2d.h"

#include <cassert>
#include <algorithm>

Spline2D::Spline2D(
        int k,
        Node_e node_type)
    :
      _node_type(node_type),
      _k(k),
      _point( _k ),
      _vec( _k-1 ),
      _node( _k + _point.size() )
{
    assert_splines();
}

// -----------------------------------------------------------------------------

void Spline2D::set_ctrl_points(const std::vector<QVector3D>& point)
{
    _point = point;
    _vec.resize(_point.size() - 1);
    for(int i = 0; i < (int)_vec.size(); ++i)
        _vec[i] = _point[i + 1] - _point[i];
    set_nodal_vector();
    assert_splines();

    for(int i = 0; i < (int)_vec.size(); ++i)
        _vec[i] /= _node[_k+i] - _node[i+1];
}

// -----------------------------------------------------------------------------

void Spline2D::get_ctrl_points(std::vector<QVector3D>& points) const
{
    points = _point;
}

// -----------------------------------------------------------------------------

/// The the nodal vector type
void Spline2D::set_node_type( Node_e type)
{
    _node_type = type;
    set_nodal_vector();
    assert_splines();
}

// -----------------------------------------------------------------------------

QVector3D Spline2D::eval_f(float u) const
{
    u = std::max(std::min(u, (float)1), (float)0); // clamp between [0 1]
    return eval(u, _point, _k, _node);
}

// -----------------------------------------------------------------------------

QVector3D Spline2D::eval_df(float u) const
{
    u = std::max(std::min(u, (float)1), (float)0); // clamp between [0 1]
    return eval(u, _vec, (_k-1), _node, 1) * (float)(_k-1);
}

// -----------------------------------------------------------------------------

void Spline2D::assert_splines() const
{
    assert( _k > 1);
    assert((int)_point.size() >= _k );
    assert(_node. size() == (_k + _point.size()) );
    assert(_point.size() == (_vec.size()    + 1) );
}

// -----------------------------------------------------------------------------

void Spline2D::set_nodal_vector()
{
    if( _node_type == eOPEN_UNIFORM)
        set_node_to_open_uniform();
    else if( _node_type == eUNIFORM )
        set_node_to_uniform();
}

// -----------------------------------------------------------------------------

void Spline2D::set_node_to_uniform()
{
    const int n = _point.size() - 1;
    _node.resize( _k + n + 1 );

    float step = (float)1 / (float)(n-_k+2);
    for (int i = 0; i < (int)_node.size(); ++i){
        _node[i] = ((float)i) * step  - step * (float)(_k-1);
    }
}

// -----------------------------------------------------------------------------

void Spline2D::set_node_to_open_uniform()
{
    _node.resize( _k + _point.size() );

    int acc = 1;
    for (int i = 0; i < (int)_node.size(); ++i)
    {
        if(i < _k)
            _node[i] = 0.;
        else if( i >= ((int)_point.size() + 1) )
            _node[i] = 1.;
        else{
            _node[i] = (float)acc / (float)(_point.size() + 1 - _k);
            acc++;
        }
    }
}

// -----------------------------------------------------------------------------

QVector3D  Spline2D::

eval(float u,
     const std::vector<QVector3D>& point,
     int k,
     const std::vector<float>& node,
     int off) const
{
    assert( k > 1);
    assert((int)point.size() >= k );
    assert_splines();
    int dec = 0;
    // TODO: better search with dychotomi ?
    // TODO: check for overflow
    while( u > node[dec + k + off] )
        dec++;

    // TODO: use buffers in attributes for better performances ?
    std::vector<QVector3D> p_rec(k, QVector3D());
    for(int i = dec, j = 0; i < (dec + k); ++i, ++j)
        p_rec[j] = point[i];

    std::vector<float> node_rec(k + k - 2, (float)0);
    for(int i = (dec + 1), j = 0; i < (dec + k + k - 1); ++i, ++j)
        node_rec[j] = node[i + off];

    return eval_rec(u, p_rec, k, node_rec);
}

// -----------------------------------------------------------------------------

QVector3D Spline2D::

eval_rec(float u,
         std::vector<QVector3D> p_in,
         int k,
         std::vector<float> node_in) const
{
    if(p_in.size() == 1)
        return p_in[0];

    // TODO: use buffers in attributes for better performances ?
    std::vector<QVector3D> p_out(k - 1,  QVector3D());
    for(int i = 0; i < (k - 1); ++i)
    {
        const float n0 = node_in[i + k - 1];
        const float n1 = node_in[i];
        const float f0 = (n0 - u) / (n0 - n1);
        const float f1 = (u - n1) / (n0 - n1);

        p_out[i] = p_in[i] *  f0 + p_in[i + 1] * f1;
    }

    std::vector<float> node_out(node_in.size() - 2);
    for(int i = 1, j = 0; i < ((int)node_in.size()-1); ++i, ++j)
        node_out[j] = node_in[i];

    return eval_rec(u, p_out, (k - 1), node_out);
}

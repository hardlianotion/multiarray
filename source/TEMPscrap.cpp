error: cannot resolve overloaded function 'begin' based on conversion to type 'size_t*'

instantiated from 

void 
tmultiarray<double, 2ul, double*, size_t, ptrdiff_t, true, trectlayoutref<2ul, size_t, ptrdiff_t> >::
reset(
	 tindexeddata<double, double*, size_t, ptrdiff_t>::iterator, const trectlayoutref<2ul, size_t, ptrdiff_t>&
)

no matching function for call to 
void
tmultiarray<
	double, 2ul, double*, size_t, ptrdiff_t, true, trectlayoutref<2ul, size_t, ptrdiff_t> 
>::reset(double*&, double*)


candidates are: 
void 
tmultiarray<
	double, 2ul, double*, size_t, ptr_diff, true, trectlayoutref<2ul, size_t, ptrdiff_t> 
>::reset( double*, const trectlayoutref<2ul, size_t, ptrdiff_t>&)


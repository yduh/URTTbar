#! /bin/env bash

shape_dir=$1
categories=$2
samples=$3

echo "add_bbb.sh"
echo $shape_dir
echo $categories
echo $samples

pushd $shape_dir

for category in $categories; do
		for sample in $samples; do
				echo "add_stat_shapes.py shapes.root shapes.root --filter $category/$sample --prefix $category --threshold 0.05"
				bbb_added=$(add_stat_shapes.py shapes.root shapes.root --filter $category/$sample --prefix $category --threshold 0.05 | sed 's|\x1b\[?1034h||g')
				if [ -n "$bbb_added" ]; then
						echo >> unc.conf
						echo >> unc.vals
						echo "#BBB uncertainties" >> unc.conf
						echo "#BBB uncertainties" >> unc.vals
						for unc in $bbb_added; do
								echo $unc shape >> unc.conf
								echo $category $sample $unc 1.0 >>  unc.vals    
						done
				fi
		done
done

sed -i 's|\x1b\[?1034h||g' unc.vals
sed -i 's|\x1b\[?1034h||g' unc.conf

popd
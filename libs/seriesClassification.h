//
//  seriesClassification.h
//  RapidLib
//
//  Created by Michael Zbyszynski on 08/06/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//

#ifndef seriesClassification_hpp
#define seriesClassification_hpp

#include <vector>
#include <string>
#include <map>
#include "fastDTW.h"
#include "trainingExample.h"

/** Class for containing time series classifiers.
 *
 * Currently only (fast)DTW.
 */

template<typename T>
class seriesClassification {
public:
    
    /** Constructor, no params */
    seriesClassification();
    ~seriesClassification();
    
    /**  Train on a specified set of trainingSeries
     * @param std::vector<trainingSeries> A vector of training series
     */
    bool train(const std::vector<trainingSeries<T> > &seriesSet);
    
    /** Reset model to its initial state, forget all costs and training data*/
    void reset();
    
    /** Compare an input series to the stored training series
     * @param std::vector<std::vector> vector of vectors, either float or double input data
     * @return The label of the closest training series.
     */
    std::string run(const std::vector<std::vector<T> > &inputSeries);
    
    /** Compare an input series to all of the stored series with a specified label
     * @param std::vector<std::vector> either float or double input data
     * @param String label to compare with
     * @return The lowest cost match, float or double
     */
    T run(const std::vector<std::vector<T> > &inputSeries, std::string label);
    
    /** Get the costs that were calculated by the run method
     * @return A vector of floats or doubles, the cost of matching to each training series
     */
    std::vector<T> getCosts() const;
    
    /** Get minimum training series length
     * @return The minimum length training series
     */
    int getMinLength() const;
    
    /** Get minimum training series length from a specified label
     * @param string The label to check
     * @return The minimum length training series of that label
     */
    int getMinLength(std::string label) const;
    
    /** Get maximum training series length
     * @return The maximum length training series
     */
    int getMaxLength() const;
    
    /** Get maximum training series length from a specified label
     * @param string The label to check
     * @return The maximum length training series of that label
     */
    int getMaxLength(std::string label) const;
    
    /** Return struct for calculate costs */
    template<typename TT>
    struct minMax {
        TT min;
        TT max;
    };
    
    /** Calculate minimum and maximum cost between examples in a label.
     * @param string Label to calculate
     * @return minMax struct containing min and max
     */
    minMax<T> calculateCosts(std::string label) const;
    
    /** Calculate minimum and maximum cost between examples in one label and examples in a second.
     * @param string first label to compare
     * @param string second label to compare
     * @return minMax struct containing min and max
     */
    minMax<T> calculateCosts(std::string label1, std::string label2) const;
    
private:
    std::vector<trainingSeries<T > > allTrainingSeries;
    std::vector<T> allCosts;
    int maxLength;
    int minLength;
    std::map<std::string, minMax<int> > lengthsPerLabel;
};

#endif

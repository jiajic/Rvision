void _plus(Image& image1, Image& image2, Image& target) {
  cv::add(image1.image, image2.image, target.image, cv::noArray(), -1);
}

void _plusScalar(Image& image, Rcpp::NumericVector value, Image& target) {
  cv::add(image.image, col2Scalar(value), target.image, cv::noArray(), -1);
}

void _minus(Image& image1, Image& image2, Image& target) {
  cv::subtract(image1.image, image2.image, target.image, cv::noArray(), -1);
}

void _minusScalar(Image& image, Rcpp::NumericVector value, bool order, Image& target) {
  if (order) {
    cv::subtract(image.image, col2Scalar(value), target.image, cv::noArray(), -1);
  } else {
    cv::subtract(col2Scalar(value), image.image, target.image, cv::noArray(), -1);
  }
}

void _multiply(Image& image1, Image& image2, Image& target) {
  cv::multiply(image1.image, image2.image, target.image, 1, -1);
}

void _multiplyScalar(Image& image, Rcpp::NumericVector value, Image& target) {
  cv::multiply(image.image, col2Scalar(value), target.image, 1, -1);
}

void _divide(Image& image1, Image& image2, Image& target) {
  cv::divide(image1.image, image2.image, target.image, 1, -1);
}

void _divideScalar(Image& image, Rcpp::NumericVector value, bool order, Image& target) {
  if (order) {
    cv::divide(image.image, col2Scalar(value), target.image, 1, -1);
  } else {
    cv::divide(col2Scalar(value), image.image, target.image, 1, -1);
  }
}

void _absdiff(Image& image1, Image& image2, Image& target) {
  cv::absdiff(image1.image, image2.image, target.image);
}

void _addWeighted(Image& image1, double alpha, Image& image2, double beta, Image& target) {
  alpha = alpha / (alpha + beta);
  beta = 1 - alpha;
  cv::addWeighted(image1.image, alpha, image2.image, beta, 0, target.image);
}
